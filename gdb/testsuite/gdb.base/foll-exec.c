/* This test program is part of GDB, the GNU debugger.

   Copyright 1997-2025 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <assert.h>
#include <limits.h>

int global_i = 100;

#ifndef EXECD_PROG
#define EXECD_PROG "execd-prog"
#endif

int main (int argc, char ** argv)
{
  int local_j = global_i + 1;
  int local_k = local_j + 1;
  char prog[PATH_MAX];
  size_t len = PATH_MAX - 1;

  printf ("foll-exec is about to execlp(%s)...\n", EXECD_PROG);

  prog [len] = '\0';

  strncpy (prog, dirname (argv[0]), len);
  len -= strlen (prog);
  assert (len > 0);

  strncat (prog, "/", len);
  len -= 1;
  assert (len > 0);

  strncat (prog, EXECD_PROG, len);
  len -= strlen (EXECD_PROG);
  assert (len > 0);

  /* In the following function call, maximum line length exceed the limit 80.
     This is intentional and required for clang compiler such that complete
     function call should be in a single line, please do not make it
     multi-line.  */
  execlp (prog, /* tbreak-execlp */ prog, "execlp arg1 from foll-exec", (char *) 0);

  printf ("foll-exec is about to execl(%s)...\n", EXECD_PROG);

  /* In the following function call, maximum line length exceed the limit 80.
     This is intentional and required for clang compiler such that complete
     function call should be in a single line, please do not make it
     multi-line.  */
  execl (prog, /* tbreak-execl */ prog, "execl arg1 from foll-exec", "execl arg2 from foll-exec", (char *) 0);

  {
    static char * argv[] = {
      (char *) "",
      (char *) "execv arg1 from foll-exec",
      (char *) 0};

    argv[0] = prog;

    printf ("foll-exec is about to execv(%s)...\n", EXECD_PROG);

    execv (prog, argv); /* tbreak-execv */
  }
}
