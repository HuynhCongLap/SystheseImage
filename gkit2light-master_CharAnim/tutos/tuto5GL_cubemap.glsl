#!/usr/bin/env python

"""Utilities for common tasks needed to use lime framework.
"""

import optparse
import subprocess
import logging
import sys
import os.path
import zipfile
import re
import shutil
import fileinput
import mimetypes
from os.path import join, splitext, split, exists
from shutil import copyfile
from datetime import datetime
import base64
import json

if sys.version_info[0]==3:
    from urllib.request import urlretrieve
else :
    from urllib import urlretrieve


basedir = os.path.dirname(os.path.dirname(os.path.realpath(__file__)))
curdir = os.path.abspath('.')

closure_dir = os.path.join(basedir,'closure')
closure_deps_file = os.path.join(closure_dir,'closure/goog/deps.js')

box2d_dir = os.path.join(basedir,'box2d')

extdir 