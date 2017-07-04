# Nicer

> ...
> `n` is for `nice`, which it really is not
> ...

A nicer `nice`.

## Why?

- `nice` would still run the command even if it can't adjust the priority.
  `nicer` will fail.
- `nice` with `sudo` requires more `sudo` to execute a command with a non-root user.
  `nicer` s a `setuid` binary and restores the original user & group id before `exec()`.
- Fun.
