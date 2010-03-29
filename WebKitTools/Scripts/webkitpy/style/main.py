# Copyright (C) 2010 Chris Jerdonek (cjerdonek@webkit.org)
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import logging
import os
import sys


_log = logging.getLogger(__name__)


# This function is a replacement for os.path.relpath(), which is only
# available in Python 2.6.  It should behave essentially the same as
# os.path.relpath(), except for returning None on paths not contained
# in abs_start_path.
def _rel_path(path, start_path, os_path_abspath=None):
    """Return a path relative to the given start path, or None.

    Returns None if the path is not contained in the directory start_path.

    Args:
      path: An absolute or relative path to convert to a relative path.
      start_path: The path relative to which the given path should be
                  converted.
      os_path_abspath: A replacement function for unit testing.  This
                       function should strip trailing slashes just like
                       os.path.abspath().  Defaults to os.path.abspath.

    """
    if os_path_abspath is None:
        os_path_abspath = os.path.abspath

    # Since os_path_abspath() calls os.path.normpath()--
    #
    # (see http://docs.python.org/library/os.path.html#os.path.abspath )
    #
    # it also removes trailing slashes and converts forward and backward
    # slashes to the preferred slash os.sep.
    start_path = os_path_abspath(start_path)
    path = os_path_abspath(path)

    if not path.lower().startswith(start_path.lower()):
        # Then path is outside the directory given by start_path.
        return None

    rel_path = path[len(start_path):]

    if not rel_path:
        # Then the paths are the same.
        pass
    elif rel_path[0] == os.sep:
        # It is probably sufficient to remove just the first character
        # since os.path.normpath() collapses separators, but we use
        # lstrip() just to be sure.
        rel_path = rel_path.lstrip(os.sep)
    else:
        # We are in the case typified by the following example:
        #
        # start_path = "/tmp/foo"
        # path = "/tmp/foobar"
        # rel_path = "bar"
        return None

    return rel_path


def change_directory(checkout_root, paths, mock_os=None):
    """Change the working directory to the WebKit checkout root, if possible.

    If every path in the paths parameter is below the checkout root (or if
    the paths parameter is empty or None), this method changes the current
    working directory to the checkout root and converts the paths parameter
    as described below.
        This allows the paths being checked to be displayed relative to the
    checkout root, and for path-specific style checks to work as expected.
    Path-specific checks include whether files should be skipped, whether
    custom style rules should apply to certain files, etc.
        If the checkout root is None or the empty string, this method returns
    the paths parameter unchanged.

    Returns:
      paths: A copy of the paths parameter -- possibly converted, as follows.
             If this method changed the current working directory to the
             checkout root, then the list is the paths parameter converted to
             normalized paths relative to the checkout root.  Otherwise, the
             paths are not converted.

    Args:
      paths: A list of paths to the files that should be checked for style.
             This argument can be None or the empty list if a git commit
             or all changes under the checkout root should be checked.
      checkout_root: The path to the root of the WebKit checkout, or None or
                     the empty string if no checkout could be detected.
      mock_os: A replacement module for unit testing.  Defaults to os.

    """
    os_module = os if mock_os is None else mock_os

    if paths is not None:
        paths = list(paths)

    if not checkout_root:
        if not paths:
            raise Exception("The paths parameter must be non-empty if "
                            "there is no checkout root.")

        # FIXME: Consider trying to detect the checkout root for each file
        #        being checked rather than only trying to detect the checkout
        #        root for the current working directory.  This would allow
        #        files to be checked correctly even if the script is being
        #        run from outside any WebKit checkout.
        #
        #        Moreover, try to find the "source root" for each file
        #        using path-based heuristics rather than using only the
        #        presence of a WebKit checkout.  For example, we could
        #        examine parent directories until a directory is found
        #        containing JavaScriptCore, WebCore, WebKit, WebKitSite,
        #        and WebKitTools.
        #             Then log an INFO message saying that a source root not
        #        in a WebKit checkout was found.  This will allow us to check
        #        the style of non-scm copies of the source tree (e.g.
        #        nightlies).
        _log.warn("WebKit checkout root not found:\n"
                  "  Path-dependent style checks may not work correctly.\n"
                  "  See the help documentation for more info.")

        return paths

    if paths:
        # Then try converting all of the paths to paths relative to
        # the checkout root.
        rel_paths = []
        for path in paths:
            rel_path = _rel_path(path, checkout_root)
            if rel_path is None:
                # Then the path is not below the checkout root.  Since all
                # paths should be interpreted relative to the same root,
                # do not interpret any of the paths as relative to the
                # checkout root.  Interpret all of them relative to the
                # current working directory, and do not change the current
                # working directory.
                _log.warn(
"""Path-dependent style checks may not work correctly:

  One of the given paths is outside the WebKit checkout of the current
  working directory:

    Path: %s
    Checkout root: %s

  Pass only files below the checkout root to ensure correct results.
  See the help documentation for more info.
"""
                          % (path, checkout_root))

                return paths
            rel_paths.append(rel_path)
        # If we got here, the conversion was successful.
        paths = rel_paths

    _log.debug("Changing to checkout root: " + checkout_root)
    os_module.chdir(checkout_root)

    return paths
