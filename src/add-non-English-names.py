#!/usr/bin/env python3
# encoding: utf-8

import os
import contextlib
import argparse
from fontTools import ttLib
from fontTools.ttLib.tables._n_a_m_e import NameRecord

def main():
  parser = argparse.ArgumentParser(description='A tool to add non‐English names to typefaces.')
  parser.add_argument('--ta')
  parser.add_argument('--sa')
  parser.add_argument('typeface_file')
  args = parser.parse_args()
  path = os.path.abspath(args.typeface_file)

  with contextlib.closing(ttLib.TTFont(path)) as ttf:
    if 'name' not in ttf:
      return

    myRecord = NameRecord()
    myRecord.nameID = 4
    myRecord.platformID = 3
    myRecord.platEncID = 10
    myRecord.langID = 1097
    myRecord.string = args.ta
    ttf['name'].names.append(myRecord)

    myRecord = NameRecord()
    myRecord.nameID = 4
    myRecord.platformID = 3
    myRecord.platEncID = 10
    myRecord.langID = 1103
    myRecord.string = args.sa
    ttf['name'].names.append(myRecord)

    ttf.save(path)

if __name__ == '__main__':
  main()
