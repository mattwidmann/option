option
======

- call `options_parse` with `argc`, `argv`, and an `option_list_t`

`option_t`
==========

- when parsing arguments, calls callback function for recognized options

`option_callback_t`
-------------------

- the corresponding `option_t`
- the position in the `argv` array
- any argument for the option, or `NULL` if no argument is expected
- user data `void *`

examples
========

    cmd -a argument -bcd argument

    cmd -aargument -bcdargument

    cmd --alpha argument --beta --gamma

    cmd --alpha=argument --beta= --gamma

