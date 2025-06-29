#!/bin/sh
# Test heredoc parsing with quoted delimiters and nested pipelines
set -e

# Quoted delimiter should disable expansions
printf 'cat << "$USER"\nwhy\nnot\n$USER\nexit\n' | ./minishell

# Nested pipeline heredocs
printf 'ls | cat << stop | ls -la | cat << stop1 | ls | cat << stop2 | ls -la | cat << stop3\n$USER\nad\nas $HOME\nstop\nawd\nwf$PWDdqwdwqd\nstop1\ndas\ndas\nstop2\ndsq\nwd\nwf$PWDdqwdwqd\nstop3\nexit\n' | ./minishell
