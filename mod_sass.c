/*
**  mod_sass.c -- Apache sass module
**
**  Then activate it in Apache's httpd.conf file:
**
**    # httpd.conf
**    LoadModule sass_module modules/mod_sass.so
**    <IfModule sass_module>
**      AddHandler sass-script .scss
**      SassCompressed   Off (On | Off)
**      SassOutput       Off (On | Off)
**      SassIncludePaths path/to/sass
**    </IfModule sass_module>
*/

/* httpd */
#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "http_main.h"
#include "http_log.h"
#include "ap_config.h"
#include "apr_strings.h"

/* libsass */
#include "libsass/sass_interface.h"

#ifdef HAVE_CONFIG_H
#  undef PACKAGE_NAME
#  undef PACKAGE_STRING
#  undef PACKAGE_TARNAME
#  undef PACKAGE_VERSION
#  include "config.h"
#endif

/* log */
#ifdef AP_SASS_DEBUG_LOG_LEVEL
#define SASS_DEBUG_LOG_LEVEL AP_SASS_DEBUG_LOG_LEVEL
#else
#define SASS_DEBUG_LOG_LEVEL APLOG_DEBUG
#endif

#define _RERR(r, format, args...)                                       \
    ap_log_rerror(APLOG_MARK, APLOG_CRIT, 0,                            \
                  r, "[SASS] %s(%d): "format, __FILE__, __LINE__, ##args)
#define _SERR(s, format, args...)                                       \
    ap_log_error(APLOG_MARK, APLOG_CRIT, 0,                             \
                 s, "[SASS] %s(%d): "format, __FILE__, __LINE__, ##args)
#define _PERR(p, format, args...)                                       \
    ap_log_perror(APLOG_MARK, APLOG_CRIT, 0,                            \
                  p, "[SASS] %s(%d): "format, __FILE__, __LINE__, ##args)

#define _RDEBUG(r, format, args...)                                     \
    ap_log_rerror(APLOG_MARK, SASS_DEBUG_LOG_LEVEL, 0,                  \
                  r, "[SASS_DEBUG] %s(%d): "format, __FILE__, __LINE__, ##args)
#define _SDEBUG(s, format, args...)                                     \
    ap_log_error(APLOG_MARK, SASS_DEBUG_LOG_LEVEL, 0,                   \
                 s, "[SASS_DEBUG] %s(%d): "format, __FILE__, __LINE__, ##args)
#define _PDEBUG(p, format, args...)                                     \
    ap_log_perror(APLOG_MARK, SASS_DEBUG_LOG_LEVEL, 0,                  \
                  p, "[SASS_DEBUG] %s(%d): "format, __FILE__, __LINE__, ##args)

/* default parameter */
#define SASS_DEFAULT_CONTENT_TYPE "text/css"

/* sass dir config */
typedef struct {
    int is_compressed;
    int is_output;
    char *include_paths;
    char *image_path;
} sass_dir_config_t;

module AP_MODULE_DECLARE_DATA sass_module;

/* output css file */
static void
sass_output_file(request_rec *r, char *data)
{
    char *ext, *fbase, *fname;
    apr_status_t rc;
    apr_size_t bytes;
    apr_file_t *file = NULL;

    ext = strstr(r->filename, ".");

    if (!data || !ext) {
        return;
    }

    fbase = apr_pstrndup(r->pool, r->filename, ext - r->filename);
    fname = apr_psprintf(r->pool, "%s.css", fbase);

    rc = apr_file_open(&file, fname, APR_WRITE | APR_CREATE,
                       APR_UREAD | APR_UWRITE | APR_GREAD, r->pool);
    if (rc == APR_SUCCESS) {
        bytes = strlen(data);
        rc = apr_file_write(file, data, &bytes);
        if (rc != APR_SUCCESS) {
            _RERR(r, "Can't create/write to file: %s", fname);
        }
        apr_file_close(file);
    } else {
        _RERR(r, "Can't create/write to file: %s", fname);
    }
}

/* content handler */
static int
sass_handler(request_rec *r)
{
    int retval = OK;
    sass_dir_config_t *config;
    struct sass_file_context *context;

    if (strcmp(r->handler, "sass-script")) {
        return DECLINED;
    }

    /* content type */
    r->content_type = SASS_DEFAULT_CONTENT_TYPE;

    if (!r->header_only) {
        config = ap_get_module_config(r->per_dir_config, &sass_module);
        context = sass_new_file_context();
        if (!context) {
            return HTTP_INTERNAL_SERVER_ERROR;
        }

        context->options.include_paths = config->include_paths;
        context->options.image_path = config->image_path;
        if (config->is_compressed) {
            context->options.output_style = SASS_STYLE_COMPRESSED;
        } else {
            context->options.output_style = SASS_STYLE_NESTED;
        }
        context->input_path = r->filename;

        sass_compile_file(context);

        if (context->error_status) {
            if (context->error_message) {
                ap_rprintf(r, "%s", context->error_message);
            } else {
                ap_rputs("An error occured; no error message available.", r);
            }
            retval = HTTP_INTERNAL_SERVER_ERROR;
        } else if (context->output_string) {
            ap_rprintf(r, "%s", context->output_string);
            if (config->is_output) {
                sass_output_file(r, context->output_string);
            }
        } else {
            ap_rputs("Unknown internal error.", r);
            retval = HTTP_INTERNAL_SERVER_ERROR;
        }

        sass_free_file_context(context);
    }

    return retval;
}

/* create dir config */
static void *
sass_create_dir_config(apr_pool_t *p, char *dir)
{
    sass_dir_config_t *config = apr_pcalloc(p, sizeof(sass_dir_config_t));

    if (config) {
        memset(config, 0, sizeof(sass_dir_config_t));

        config->is_compressed = 0;
        config->is_output = 0;
        config->include_paths = "";
        config->image_path = "";
    }

    return (void *)config;
}

/* merge dir config */
static void *
sass_merge_dir_config(apr_pool_t *p, void *base_conf, void *override_conf)
{
    sass_dir_config_t *config =
        (sass_dir_config_t *)apr_pcalloc(p, sizeof(sass_dir_config_t));
    sass_dir_config_t *base = (sass_dir_config_t *)base_conf;
    sass_dir_config_t *override = (sass_dir_config_t *)override_conf;

    if (override->is_compressed != 0) {
        config->is_compressed = 1;
    } else {
        config->is_compressed = base->is_compressed;
    }

    if (override->is_output != 0) {
        config->is_output = 1;
    } else {
        config->is_output = base->is_output;
    }

    if (override->include_paths && strlen(override->include_paths) > 0) {
        config->include_paths = override->include_paths;
    } else {
        config->include_paths = base->include_paths;
    }

    if (override->image_path && strlen(override->image_path) > 0) {
        config->image_path = override->image_path;
    } else {
        config->image_path = base->image_path;
    }

    return (void *)config;
}

/* Commands */
static const command_rec sass_cmds[] =
{
    AP_INIT_FLAG("SassCompressed", ap_set_flag_slot,
                 (void *)APR_OFFSETOF(sass_dir_config_t, is_compressed),
                 RSRC_CONF|ACCESS_CONF, "sass compressed to 'on' or 'off'"),
    AP_INIT_FLAG("SassOutput", ap_set_flag_slot,
                 (void *)APR_OFFSETOF(sass_dir_config_t, is_output),
                 RSRC_CONF|ACCESS_CONF, "sass output (css) to 'on' or 'off'"),
    AP_INIT_TAKE1("SassIncludePaths", ap_set_string_slot,
                  (void *)APR_OFFSETOF(sass_dir_config_t, include_paths),
                  RSRC_CONF|ACCESS_CONF, "sass include paths"),
    AP_INIT_TAKE1("SassImagePath", ap_set_string_slot,
                  (void *)APR_OFFSETOF(sass_dir_config_t, image_path),
                  RSRC_CONF|ACCESS_CONF, "sass image path"),
    { NULL }
};


/* Hooks */
static void sass_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(sass_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Module */
module AP_MODULE_DECLARE_DATA sass_module =
{
    STANDARD20_MODULE_STUFF,
    sass_create_dir_config,  /* create per-dir    config structures */
    sass_merge_dir_config,   /* merge  per-dir    config structures */
    NULL,                    /* create per-server config structures */
    NULL,                    /* merge  per-server config structures */
    sass_cmds,               /* table of config file commands       */
    sass_register_hooks      /* register hooks                      */
};
