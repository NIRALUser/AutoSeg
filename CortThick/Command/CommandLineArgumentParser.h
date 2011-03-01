/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: CommandLineArgumentParser.h,v $
  Language:  C++
  Date:      $Date: 2009/01/26 20:15:55 $
  Version:   $Revision: 1.1 $
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#ifndef __CommandLineArgumentParser_h_
#define __CommandLineArgumentParser_h_

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#pragma warning ( disable : 4503 )
#endif

#include <vector>
#include <string>
#include <list>
#include <map>

/**
 * \class CommandLineArgumentParseResult
 * \brief Object returned by CommandLineArgumentParser
 * \see CommandLineArgumentParser
 */
class CommandLineArgumentParseResult
{
public:
  /** Check whether the option was passed in or not */
  bool IsOptionPresent(const char *option);

  /** Get one of the parameters to the option */
  const char *GetOptionParameter(char *option, unsigned int number = 0);

private:
  typedef std::vector< std::string > ParameterArrayType;
  typedef std::map< std::string, ParameterArrayType> OptionMapType;

  void Clear();
  void AddOption(const std::string &option, int nParms);
  void AddParameter(const std::string &option, const std::string &parameter);

  OptionMapType m_OptionMap;

  friend class CommandLineArgumentParser;
};

/**
 * \class CommandLineArgumentParser
 * \brief Used to parse command line arguments and come back with a list
 * of parameters.
 * Usage:
 * \code
 *    // Set up the parser
 *    CommandLineArgumentParser parser;
 *    parser.AddOption("-f",1);
 *    parser.AddSynonim("-f","--filename");
 *    parser.AddOption("-v",0);
 *    parser.AddSynonim("-v","--verbose");
 *
 *    // Use the parser
 *    CommandLineArgumentParseResult result;
 *    if(parser.TryParseCommandLine(argc,argv,result)) {
 *       if(result.IsOptionPresent("-f"))
 *          cout << "Filename " << result.GetOptionParameter("-f") << endl;
 *       ...
 *    } 
 * \endcode      
 */
class CommandLineArgumentParser
{
public:
  /** Add an option with 0 or more parameters (words that follow it) */
  void AddOption(const char *name, int nParameters);
  
  /** Add a different string that envokes the same option (--file and -f) */  
  void AddSynonim(const char *option, const char *synonim);

  /** Try processing a command line.  Returns false if something breaks */
  bool TryParseCommandLine(int argc, char *argv[], 
                           CommandLineArgumentParseResult &outResult,
                           bool failOnUnknownTrailingParameters = true);
private:
  // Synonim list type
  typedef std::list< std::string > NameListType;
  typedef struct 
    {
    std::string CommonName;
    unsigned int NumberOfParameters;
    } OptionType;
  typedef std::map< std::string, OptionType> OptionMapType;

  OptionMapType m_OptionMap;
};

#endif // __CommandLineArgumentParser_h_
