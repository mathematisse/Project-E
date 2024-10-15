# Contributing to RType

Here are the guidelines to help you get started:

## How to Contribute

1. **Create an Issue**: Before starting any work, please create an issue to discuss the changes you plan to make.
2. **Create a Branch**: Once the issue is approved, create a new branch for your work.
3. **Push Your Changes**: Commit and push your changes to the branch.
4. **Open a Pull Request**: When your work is ready, open a pull request for review.

## Commit Message Guidelines

To maintain a clear and consistent commit history, please follow these guidelines for commit messages:

### Allowed Verbs

Your commit message must start with one of the following verbs:

- Add
- Edit
- Remove
- Document
- Test
- Refactor
- Fix
- Merge

### Example

```
Add new feature
Fix issue with X
```

### Validation Script

A script is provided to ensure your commit messages follow the guidelines:

```bash
allowed_verbs="Add|Edit|Remove|Document|Test|Refactor|Fix|Merge"

commit_message=$(head -n 1 "$1")

if [[ ! "$commit_message" =~ ^($allowed_verbs) ]]; then
    echo "Error: Commit message must start with one of the following verbs: $allowed_verbs"
    echo "Example: 'Add new feature' or 'Fix issue with X'"
    exit 1
fi
```

you can find this script in [scripts/hook-commit.sh](./scripts/hook-commit.sh). And install the commit hook by running the following command:

```bash
./scripts/install-hooks.sh
```
