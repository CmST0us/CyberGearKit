//
//  visibility.h
//  test_bit
//
//  Created by Eric Wu on 2023/9/5.
//

#pragma once

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define CYBERGEARAPI __attribute__ ((dllexport))
    #else
      #define CYBERGEARAPI __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define CYBERGEARAPI __attribute__ ((dllimport))
    #else
      #define CYBERGEARAPI __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define CYBERGEARAPI __attribute__ ((visibility ("default")))
    #define CYBERGEARHIDE  __attribute__ ((visibility ("hidden")))
  #else
    #define CYBERGEARAPI
    #define CYBERGEARHIDE
  #endif
#endif
