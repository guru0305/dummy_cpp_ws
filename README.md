# ROS2 Dummy C++ Workspace — CI Enforcement Architecture

This repository demonstrates a structured CI/CD workflow for a ROS2 Humble C++ workspace. It implements local validation, remote enforcement, branch protection, and Docker-based CI optimization to ensure code quality and repository integrity.

---

## Repository Structure

### Branches

**`main`** — Protected production branch. Direct pushes are blocked and all changes must go through a Pull Request. Merging requires all CI status checks to pass.

**`ci-image`** — Dedicated branch where the Docker-based CI workflow is executed. The prebuilt ROS2 Humble image is consumed here exclusively for workspace build and test validation.

**`divide`** — Contains a working division function. All tests pass and the branch serves as a valid CI scenario reference.

**`square`** — Contains a square function with intentionally failing tests. Used to validate that the CI pipeline correctly blocks a failing Pull Request.

**`no_test_dir_pkg`** — Contains a package without a `test/` directory. Used to validate enforcement of the test directory requirement.

**`no_test_pkg`** — Contains a package with a `test/` directory but no valid test files. Used to validate enforcement of the minimum test execution requirement.

---

## Development Workflow

1. Clone the repository.
2. Create a feature branch from `main`.
3. Implement changes.
4. Push to the feature branch.
5. Open a Pull Request targeting `main`.
6. GitHub Actions runs all validation checks.
7. If all checks pass, merging is allowed.
8. If any check fails, merging is blocked.

---

## Local Validation

### Pre-Push Hook

A Git pre-push hook is used to validate builds and tests before code reaches the remote repository.

**Location:** `.git/hooks/pre-push`

```bash
#!/bin/bash

echo "Running colcon build..."
source /opt/ros/humble/setup.bash

colcon build
if [ $? -ne 0 ]; then
  echo "Build failed. Push aborted."
  exit 1
fi

echo "Running colcon test..."
colcon test
colcon test-result --verbose
if [ $? -ne 0 ]; then
  echo "Tests failed. Push aborted."
  exit 1
fi

echo "All tests passed. Proceeding with push."
exit 0
```

This hook ensures the build succeeds and all tests pass before a push is allowed.

> **Note:** Local hooks can be bypassed using `git push --no-verify`. Local validation improves developer workflow but does not constitute repository-level enforcement.

---

## Remote Enforcement

### Branch Protection and CI

The `main` branch is protected using GitHub branch protection rules:

- Direct pushes are blocked.
- Pull Requests are required for all changes.
- Required status checks must pass before a PR can be merged.

All enforcement is handled remotely through GitHub Actions, independent of local developer configuration.

---

## CI Architecture

The repository uses two CI workflows.

### Docker Image Builder

This workflow builds a preconfigured ROS2 Humble Docker image used as the base environment for all CI runs.

The image includes:

- Ubuntu 22.04
- ROS2 Humble
- Colcon
- Required build tools

The image is rebuilt only when the Dockerfile changes, keeping subsequent CI runs efficient.

### Docker Workspace CI

This workflow runs on the `ci-image` branch and uses the prebuilt Docker image:

```yaml
container:
  image: ghcr.io/<repository>:ci-test
```

Using a prebuilt image avoids reinstalling ROS2 on every run, reducing CI execution time and ensuring a consistent, reproducible environment across all executions.

---

## CI Validation Rules

During Pull Request validation, the following checks are enforced:

| # | Rule |
|---|---|
| 1 | Every ROS2 package must contain a `test/` directory. |
| 2 | Every package must execute at least one valid test. |
| 3 | `colcon build` must succeed without errors. |
| 4 | `colcon test` must pass with zero failures. |

Failure of any condition blocks the Pull Request from merging into `main`.

---

## Why Docker

| Without Docker | With Docker |
|---|---|
| ROS2 must be installed on every CI run. | ROS2 is preinstalled in the image. |
| CI runtime increases with each dependency install. | CI runs are significantly faster. |
| Environment may vary between runs. | Environment is fully deterministic. |
| Higher risk of inconsistent results. | Consistent results across all executions. |

---

## Additional CI Enhancements

The architecture supports extending validation with the following:

- **Secret scanning** — detect API keys or credentials committed to the repository.
- **Lint checks** — enforce formatting, naming conventions, and code style.
- **Static analysis** — identify memory misuse, null dereferences, and uninitialized variables.
- **Compiler warnings as errors** — surface issues at build time.
- **Copyright header validation** — ensure all source files include required license headers.
- **Dependency validation** — verify consistency between `package.xml` and `CMakeLists.txt`.
- **Code coverage reporting** — track test coverage over time.

---