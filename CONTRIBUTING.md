# Contributing to NLOptWR

The NLOptWR team encourages community feedback and contributions.
Thank you for your interest in making NLOptWR better!

There are several
ways you can get involved.

## Reporting issues and suggesting new features

If you find that the project is not working properly, please file a report.
But label the Bug Report accordingly.

We are happy to hear your ideas for how to further improve NLOptWR,
ensuring it suits your needs.
We look at upvoted items in [Issues](https://github.com/knut-o/nloptwr/issues) and when we decide what to work on next.
We read the comments and we look forward to hearing your input.

## Contributions we accept

We highly appreciate any contributions that help us improve the end product, with
a high emphasis being put on any bug fixes you can manage to create and direct
improvements which address the top issues reported by Calculator users. Some general
guidelines:

### DOs

* **DO** create one pull request per Issue, and ensure that the Issue is linked in the pull request.

* **DO** follow our [Coding and Style](#style-guidelines) guidelines, and keep code
changes as small as possible.

* **DO** include corresponding tests whenever possible.

* **DO** check for additional occurrences of the same problem in other parts of the
codebase before submitting your PR.

* **DO** link the issue you are addressing in the pull request.

* **DO** write a good description for your pull request. More detail is better.DO NOT
Describe *why* the change is being made and *why* you have chosen a particular solution.
Describe any manual testing you performed to validate your change.

### DO NOTs

* **DO NOT** merge multiple changes into one PR unless they have the same root cause.
* **DO NOT** merge directly into the master branch.

> Submitting a pull request for an approved Issue is not a guarantee it will be approved.
> The change must meet our high bar for code quality, architecture and performance.

## Making changes to the code

### Preparing your development environment

To learn how to build the code and run tests, follow the instructions in the [README](README.md).

### Style guidelines

The code should contain comments which can be used
by [doxygen](https://doxygen.nl/) for documentation.

### Code formatting

***Run clang-format***

Use the following commands from the project's root directory to run clang-format
(must be installed on the host system).

**1. Run the CMake target for `clang-format`:**

```bash
cmake --build build --target clangformat
```

**2. Using clang-format:**

```bash
# clang-format does not run recursively in subdirectories
# for each .cpp file modified
clang-format -i *.cpp

# for each .h file modified
clang-format -i *.h
```

### Testing

Tests can made with [ctest](https://cmake.org/cmake/help/latest/module/CTest.html). In the file ***CMakeLists.txt*** are already some examples.

### Git workflow

The core principle of the project, when it comes to Git workflows is that the
`main` branch should always be in a healthy state which is ready for release.
Every commit on master should be deployable on push.
To ensure this, pull request
**must not** be made directly on master.
**Each change** should either be made in the **development branch**
(named a variation of development, i.e. `dev`)
or in a separate branch,
named as a short summary of the change.

If your change is complex, please clean up the branch history before submitting a
pull request. You can use [git rebase](https://git-scm.com/book/en/v2/Git-Branching-Rebasing)
to group your changes into a small number of commits which we can review one at a
time.

When completing a pull request, we will generally squash your changes into a single
commit. After confirming that the change works as intended, the branch *might* be
deleted, in order to prevent branch polluting. Please let us know if your pull request
needs to be merged as separate commits.

### Continuous Integration

For this project, CI is provided by [GitHub Actions](https://github.com/features/actions),
with workflows found in the [`.github/workflows` folder](.github/workflows). Workflows
are run automatically on every commit made on the master branch, unless told to skip
for that particular commit.

To skip CI runs on a particular commit, include either `[skip ci]` or `[ci skip]`
in the commit message.

```bash
# an example of a commit message that would not trigger CI workflows
git commit -m "my normal commit message [skip ci]"
# or
git commit -m "my normal commit message [ci skip]"
```

## Review process

After submitting a pull request, members of the team will review your code.

We will assign the request to an appropriate reviewer (if applicable).
Any member of the community may participate in the review, but at least one member of the project team will ultimately approve the request.

Often, multiple iterations or discussions will be needed to responding to feedback from reviewers.
