# mod_sass

mod_sass is Sass handler module for Apache HTTPD Server.

## Dependencies

* [libsass](https://github.com/hcatlin/libsass/)

## Build

    % ./autogen.sh (or autoreconf -i)
    % ./configure [OPTION]
    % make
    % make install

### Build Options

apache path.

* --with-apxs=PATH  [default=yes]
* --with-apr=PATH  [default=yes]

## Configration

httpd.conf:

    # Load module
    LoadModule sass_module modules/mod_sass.so

    # Handler sass script
    AddHandler sass-script .scss

    # Output compressed (minify) [On | Off]
    SassCompressed On

    # Output to CSS file [On | Off]
    SassOutput Off

    # Include paths [PATH]
    SassIncludePaths path/to/inc

    # Image Path [PATH]
    SassImagePath path/to/img

## Example

example.scss:

    // Variables
    $blue: #3bbfce;
    $margin: 16px;

    .content-navigation {
      border-color: $blue;
      color:
        darken($blue, 9%);
    }

    .border {
      padding: $margin / 2;
      margin: $margin / 2;
      border-color: $blue;
    }

    // Nesting
    table.hl {
      margin: 2em 0;
      td.ln {
        text-align: right;
      }
    }

    li {
      font: {
        family: serif;
        weight: bold;
        size: 1.2em;
      }
    }

    // Mixins
    @mixin table-base {
      th {
        text-align: center;
        font-weight: bold;
      }
      td, th {padding: 2px}
    }

    @mixin left($dist) {
      float: left;
      margin-left: $dist;
    }

    #data {
      @include left(10px);
      @include table-base;
    }

    // Selector Inheritance
    .error {
      border: 1px #f00;
      background: #fdd;
    }
    .error.intrusion {
      font-size: 1.3em;
      font-weight: bold;
    }

    .badError {
      @extend .error;
      border-width: 3px;
    }

**Output:**

    .content-navigation {
      border-color: #3bbfce;
      color: #2ca2af; }

    .border {
      padding: 8px;
      margin: 8px;
      border-color: #3bbfce; }

    table.hl {
      margin: 2em 0; }
      table.hl td.ln {
        text-align: right; }

    li {
      font-family: serif;
      font-weight: bold;
      font-size: 1.2em; }

    #data {
      float: left;
      margin-left: 10px; }
      #data th {
        text-align: center;
        font-weight: bold; }
      #data td, #data th {
        padding: 2px; }

    .error, .badError {
      border: 1px #f00;
      background: #fdd; }

    .error.intrusion, .intrusion.badError {
      font-size: 1.3em;
      font-weight: bold; }

    .badError {
      border-width: 3px; }
