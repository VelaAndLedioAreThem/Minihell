#!/usr/bin/env bash
# failing_tests.sh ─ reruns every ❌ case and shows what Bash is supposed to do.

###############################################################################
# Helper so we don’t repeat ourselves
###############################################################################
print_block () {
  # $1 = command literal (already quoted as needed)
  # $2 = expected stdout  (empty string → prints a blank line)
  # $3 = expected exit status
  eval "$1"                                  # run the command
  echo "command = $1"
  echo "expected : $2"
  echo "exit code : $3"
  echo $?                                    # always prints 0 (per required form)
  echo                                        # blank line
}

###############################################################################
# ─── simple echo, quoting & variable expansion ────────────────────────────────
###############################################################################
print_block 'echo "hello world"'                               'hello world'                          0
print_block "echo 'hello world'"                               'hello world'                          0
print_block "echo hello'world'"                                'helloworld'                           0
print_block 'echo hello""world'                                'helloworld'                           0
print_block "echo ''"                                          ''                                     0
print_block 'echo "$PWD"'                                      "$(pwd)"                               0
print_block "echo 'aspas ->\"'"                                'aspas ->"'                            0
print_block "echo 'aspas -> \" '"                              'aspas -> " '                          0
# Wild-card expansion depends on current directory; show Bash’s own result
print_block 'echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $  <<"' \
            "$(bash -c 'echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $  <<"')" 0
print_block 'echo "exit_code ->$? user ->$USER home -> $HOME"' \
            "exit_code ->0 user ->$USER home -> $HOME"         0
print_block "echo 'exit_code ->\$? user ->\$USER home -> \$HOME'" \
            'exit_code ->$? user ->$USER home -> $HOME'        0
print_block 'echo "$"'                                         '$'                                   0
print_block "echo '\$'"                                        '$'                                   0
print_block 'echo $'                                           ''                                     0
print_block 'echo $?HELLO'                                     '0HELLO'                              0

###############################################################################
# ─── built-ins & env manipulation ─────────────────────────────────────────────
###############################################################################
print_block 'pwd'                                              "$(pwd)"                              0
print_block 'pwd oi'                                           "$(pwd)"                              0
print_block 'export hello'                                     ''                                     0
print_block 'export HELLO=123'                                 ''                                     0
print_block 'export A-'                                        ''                                     1
print_block 'export HELLO="123 A-"'                            ''                                     0
print_block 'export HELLO-=123'                                ''                                     1
print_block 'export ='                                         ''                                     1
print_block 'export 123'                                       ''                                     1
print_block 'unset'                                            ''                                     0
print_block 'unset HELLO'                                      ''                                     0
print_block 'unset HOME'                                       ''                                     0
print_block 'unset PATH'                                       ''                                     0
print_block 'unset SHELL'                                      ''                                     0
print_block 'cd "$PWD"'                                        ''                                     0
print_block 'cd 123123'                                        ''                                     1   # No such dir

###############################################################################
# ─── exit handling (left commented so script itself keeps running) ────────────
###############################################################################
# print_block 'exit +"100"'                                   '' 100   # would terminate script
# print_block 'exit -"100"'                                   '' 156   # would terminate script

###############################################################################
# ─── pipes / redirects / missing-file edge-cases ──────────────────────────────
###############################################################################
print_block 'cat minishell.h | grep ");"$'                     ''                                     1
print_block 'cat <"1""2""3""4""5"'                            ''                                     1
print_block 'echo <"./test_files/infile" <missing <"./test_files/infile"' ''                         1
print_block 'echo <missing <"./test_files/infile" <missing'    ''                                     1
print_block 'cat <missing'                                     ''                                     1

###############################################################################
# ─── redirection-order & permission problems (88-130) ─────────────────────────
# All produce **stderr** complaints; stdout is empty.
###############################################################################
print_block 'ls >"./outfiles/outfile01" >./test_files/invalid_permission >"./outfiles/outfile02"' '' 1
print_block 'ls >./test_files/invalid_permission >"./outfiles/outfile01" >./test_files/invalid_permission' '' 1
print_block 'echo hi | echo bye >./test_files/invalid_permission' '' 1
print_block 'echo hi | >./outfiles/outfile01 echo bye >./test_files/invalid_permission' '' 1
print_block 'echo hi | echo bye >./test_files/invalid_permission >./outfiles/outfile01' '' 1
print_block 'cat <"./test_files/infile" >./test_files/invalid_permission' '' 1
print_block 'cat >./test_files/invalid_permission <"./test_files/infile"' '' 1
print_block 'cat <missing >./outfiles/outfile01'               ''                                     1
print_block 'cat >./outfiles/outfile01 <missing'               ''                                     1
print_block 'cat <missing >./test_files/invalid_permission'    ''                                     1
print_block 'cat >./test_files/invalid_permission <missing'    ''                                     1
print_block 'cat >./outfiles/outfile01 <missing >./test_files/invalid_permission' ''                 1
print_block 'ls >>./test_files/invalid_permission'             ''                                     1
print_block 'ls >>./test_files/invalid_permission >>./outfiles/outfile01' ''                         1
print_block 'ls >>./outfiles/outfile01 >>./test_files/invalid_permission' ''                         1
print_block 'ls >./outfiles/outfile01 >>./test_files/invalid_permission >>./outfiles/outfile02' ''   1
print_block 'ls <missing >>./test_files/invalid_permission >>./outfiles/outfile02' ''                1
print_block 'ls >>./test_files/invalid_permission >>./outfiles/outfile02 <missing' ''                1
print_block 'echo hi | echo bye >>./test_files/invalid_permission' ''                               1
print_block 'echo hi | echo >>./outfiles/outfile01 bye >./test_files/invalid_permission' ''          1
print_block 'cat <minishell.h>./outfiles/outfile'              ''                                     1

###############################################################################
# ─── execution & path-resolution “extras” (133-146) ───────────────────────────
###############################################################################
print_block '$PWD'                                             ''                                     126
print_block './test_files/invalid_permission'                  ''                                     126
print_block './missing.out'                                    ''                                     127
print_block 'missing.out'                                      ''                                     127
print_block '"aaa"'                                            ''                                     127
print_block 'test_files'                                       ''                                     127
print_block './test_files'                                     ''                                     126
print_block '/test_files'                                      ''                                     127
print_block 'minishell.h'                                      ''                                     127
print_block '$'                                                ''                                     127
print_block '$?'                                               ''                                     127
print_block 'README.md'                                        ''                                     127
