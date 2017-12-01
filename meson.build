# Copyright © 2017 Broadcom
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

project(
  'hash_table',
  'c',
  version : '1.0',
  license : 'MIT',
)

libhash_table = shared_library(
  'hash_table',
  [
    'hash_table.c',
    'fnv_hash.c',
  ],
)

libset = shared_library(
  'set',
  [
    'set.c',
    'fnv_hash.c',
  ],
)

hash_tests = [
  'collision',
  'collision_replacement',
  'delete_and_lookup',
  'delete_management',
  'deleted_key',
  'destroy_callback',
  'insert_and_lookup',
  'insert_many',
  'null_destroy',
  'random_entry',
  'remove_null',
  'replacement',
]

foreach t: hash_tests
  test(
    'hash_table-' + t,
    executable(
      'hash_table-' + t + '-prog',
      join_paths('tests', t + '.c'),
      link_with: libhash_table
    )
  )
endforeach

set_tests = [
  'collision_replacement',
  'delete_and_lookup',
  'delete_management',
  'deleted_key',
  'destroy_callback',
  'insert_and_lookup',
  'insert_many',
  'null_destroy',
  'null_remove',
  'random_entry',
  'replacement',
]

foreach t: set_tests
  test(
    'set-' + t,
    executable(
      'set-' + t + '-prog',
      join_paths('tests', 'set', t + '.c'),
      link_with: libset
    )
  )
endforeach
