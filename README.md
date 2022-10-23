<h1 align="center">
    Lyrahgames' Extended Standard
</h1>

<p align="center">
    Header-Only C++ Library to Extend the Functionality of the STL
</p>

## Development Status

<p align="center">
    <img src="https://img.shields.io/github/languages/top/lyrahgames/xstd.svg?style=for-the-badge">
    <img src="https://img.shields.io/github/languages/code-size/lyrahgames/xstd.svg?style=for-the-badge">
    <img src="https://img.shields.io/github/repo-size/lyrahgames/xstd.svg?style=for-the-badge">
    <a href="COPYING.md">
        <img src="https://img.shields.io/github/license/lyrahgames/xstd.svg?style=for-the-badge&color=blue">
    </a>
</p>

<b>
<table align="center">
    <tr>
        <td>
            master
        </td>
        <td>
            <a href="https://github.com/lyrahgames/xstd">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/xstd/master.svg?logo=github&logoColor=white">
            </a>
        </td>
        <!-- <td>
            <a href="https://circleci.com/gh/lyrahgames/xstd/tree/master"><img src="https://circleci.com/gh/lyrahgames/xstd/tree/master.svg?style=svg"></a>
        </td> -->
        <!-- <td>
            <a href="https://codecov.io/gh/lyrahgames/xstd">
              <img src="https://codecov.io/gh/lyrahgames/xstd/branch/master/graph/badge.svg" />
            </a>
        </td> -->
        <td>
            <a href="https://ci.stage.build2.org/?builds=lyrahgames-xstd&pv=&tc=*&cf=&mn=&tg=&rs=*">
                <img src="https://img.shields.io/badge/b|2 ci.stage.build2.org-Click here!-blue">
            </a>
        </td>
    </tr>
    <!-- <tr>
        <td>
            develop
        </td>
        <td>
            <a href="https://github.com/lyrahgames/xstd/tree/develop">
                <img src="https://img.shields.io/github/last-commit/lyrahgames/xstd/develop.svg?logo=github&logoColor=white">
            </a>
        </td>
        <td>
            <a href="https://circleci.com/gh/lyrahgames/xstd/tree/develop"><img src="https://circleci.com/gh/lyrahgames/xstd/tree/develop.svg?style=svg"></a>
        </td>
        <td>
            <a href="https://codecov.io/gh/lyrahgames/xstd">
              <img src="https://codecov.io/gh/lyrahgames/xstd/branch/develop/graph/badge.svg" />
            </a>
        </td>
    </tr> -->
    <tr>
        <td>
        </td>
    </tr>
    <tr>
        <td>
            Current
        </td>
        <td>
            <a href="https://github.com/lyrahgames/xstd">
                <img src="https://img.shields.io/github/commit-activity/y/lyrahgames/xstd.svg?logo=github&logoColor=white">
            </a>
        </td>
        <!-- <td>
            <img src="https://img.shields.io/github/release/lyrahgames/xstd.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/release-pre/lyrahgames/xstd.svg?label=pre-release&logo=github&logoColor=white">
        </td> -->
        <td>
            <img src="https://img.shields.io/github/tag/lyrahgames/xstd.svg?logo=github&logoColor=white">
        </td>
        <td>
            <img src="https://img.shields.io/github/tag-date/lyrahgames/xstd.svg?label=latest%20tag&logo=github&logoColor=white">
        </td>
        <!-- <td>
            <a href="https://queue.cppget.org/xstd">
                <img src="https://img.shields.io/website/https/queue.cppget.org/xstd.svg?down_message=empty&down_color=blue&label=b|2%20queue.cppget.org&up_color=orange&up_message=running">
            </a>
        </td> -->
    </tr>
</table>
</b>

## Requirements
<b>
<table>
    <tr>
        <td>Language Standard:</td>
        <td>C++20</td>
    </tr>
    <tr>
        <td>Compiler:</td>
        <td>
            GCC | Clang
        </td>
    </tr>
    <tr>
        <td>Build System:</td>
        <td>
            <a href="https://build2.org/">build2</a>
        </td>
    </tr>
    <tr>
        <td>Operating System:</td>
        <td>
            Linux
        </td>
    </tr>
    <tr>
        <td>Dependencies:</td>
        <td>
            -
        </td>
    </tr>
</table>
</b>

## Getting Started

## Usage with build2
Add this repository to the `repositories.manifest` file of your build2 package.

    :
    role: prerequisite
    location: https://github.com/lyrahgames/xstd.git

Add the following entry to the `manifest` file with a possible version dependency.

    depends: lyrahgames-xstd

Add these entries to your `buildfile`.

    import libs = lyrahgames-xstd%lib{lyrahgames-xstd}
    exe{your-executable}: {hxx cxx}{**} $libs


## Installation
The standard installation process will only install the header-only library with some additional description, library, and package files.

    bpkg -d build2-packages cc \
      config.install.root=/usr/local \
      config.install.sudo=sudo

Get the latest package release and build it.

    bpkg build https://github.com/lyrahgames/xstd.git

Install the built package.

    bpkg install lyrahgames-xstd

For uninstalling, do the following.

    bpkg uninstall lyrahgames-xstd

If your package uses an explicit `depends: lyrahgames-xstd` make sure to initialize this dependency as a system dependency when creating a new configuration.

    bdep init -C @build cc config.cxx=g++ "config.cxx.coptions=-O3" -- "?sys:lyrahgames-xstd/*"

## Alternative Usage
To use other build systems or manual compilation, you only have to add the `lyrahgames/xstd/` directory to your project and include it in the compilation process.

## API
Sadly, robust and automatic generation of API documents for the modern C++ standard is still lacking.
So, you are probably left with reading the source code files directly to get some understanding of the main API parts.
The source code provides some amount of comments to make it easier to understand.

## Examples
