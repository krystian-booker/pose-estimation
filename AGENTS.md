# Repository Guidelines

## Project Structure & Module Organization

GuessWork estimates FRC robot poses using synchronized cameras, AprilTags, stereo VIO, and sensor fusion.

- `src/`: C++ modules for frame handling (`core/`), capture (`producer/`), consumers, encoding, calibration, AprilTags, VIO, fusion, networking, and HTTP services (`server/`).
- `apps/guesswork/main.cpp`: application entry point.
- `web/src/`: React pages, reusable components, typed API clients, and query hooks; `web/public/` holds static assets.
- `tests/`: C++ unit tests; `web/e2e/`: browser tests.
- `firmware/`: PlatformIO firmware for the MicoAir F405 V2 controller.
- `data/`: field layouts and calibration targets; `docs/`: hardware and protocol references; `cmake/` and `docker/kalibr/`: dependency and calibration tooling.

## Build, Test, and Development Commands

The host application requires macOS, targets Apple Silicon, and uses system Apple frameworks. Install dependencies described in `CLAUDE.md`, including Homebrew OpenCV, Eigen, Boost, Ceres, and OpenSSL. Frontend builds require Node ≥22.12.

- `cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug`: configure with the installed Spinnaker SDK. Add `-DGW_STUB_SPINNAKER=ON` for camera-free development.
- `cmake --build build -j`: build the application and unit tests.
- `./build/guesswork`: start the backend on port 8080.
- `cd web && npm install && npm run dev`: serve the frontend on port 5173, proxying `/api` to the backend.
- `cd web && npm run build`: type-check and build frontend assets.
- `ctest --test-dir build --output-on-failure`: run GoogleTest tests.
- `cd web && npx playwright test`: run mocked browser tests without hardware.

## Coding Style & Naming Conventions

Use C++20, four-space indentation, `snake_case` filenames/functions, `PascalCase` types, and trailing underscores for private members. Preserve module namespaces and existing Pimpl boundaries around heavy SDK dependencies. TypeScript uses two-space indentation, single quotes, and no semicolons; React components use `PascalCase`. No dedicated formatter or linter configuration is checked in; match surrounding code.

## Testing Guidelines

Name C++ tests `tests/test_<module>.cpp` and register new files in `CMakeLists.txt`. Browser tests use `web/e2e/*.spec.ts`. Hardware tests use `hw.*.spec.ts`; run with `GW_E2E_HW=1` after building `build-fresh/guesswork` and connecting a camera. No coverage threshold is configured. Add regression tests for behavior changes.

## Commit & Pull Request Guidelines

History mixes imperative subjects with `feat:`, `refactor:`, and `docs:` prefixes. Keep commits focused. PRs should explain behavior changes, list validation and hardware assumptions, link relevant issues, and include screenshots for UI changes.

Read `docs/pose_pipeline.md` before changing pose math. Update protocol documentation alongside wire-format changes.
