This is where the libraries built from sub-repos should live. Keeping the build output here avoids cluttering the sub-repo trees with build files and making Git think there are uncommitted changes.

Each sub-repo should build to a corresponding directory here. The main build helper script in the root of this repo should manage this automatically.