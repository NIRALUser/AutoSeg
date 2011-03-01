/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: CommandLineArgumentParser.cxx,v $
  Language:  C++
  Date:      $Date: 2009/01/26 20:15:55 $
  Version:   $Revision: 1.1 $
  Copyright (c) 2003 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.
=========================================================================*/
#include "CommandLineArgumentParser.h"

#include <cassert>
#include <iostream>

using namespace std;

void 
CommandLineArgumentParser
::AddOption(const char *name, int nParameters)
{
  // Create a structure for the command
  OptionType option;
  option.CommonName = string(name);
  option.NumberOfParameters = nParameters;

  // Add the option to the map
  m_OptionMap[string(name)] = option;
}

void 
CommandLineArgumentParser
::AddSynonim(const char *option, const char *synonim)
{
  string strOption(option);
  string strSynonim(synonim);

  // The option should exist!
  assert(m_OptionMap.find(strOption) != m_OptionMap.end());

  // Create a new option object
  OptionType o;
  o.NumberOfParameters = m_OptionMap[strOption].NumberOfParameters;
  o.CommonName = strOption;

  // Insert the option into the map
  m_OptionMap[strSynonim] = o;
}

bool 
CommandLineArgumentParser
::TryParseCommandLine(int argc, char *argv[], 
                      CommandLineArgumentParseResult &outResult,
                      bool failOnUnknownTrailingParameters)
{
  // Clear the result
  outResult.Clear();

  // Go through the arguments
  for(int i=1; i < argc; i++)
    {
    // Get the next argument
    string arg(argv[i]);

    // Check if the argument is known
    if(m_OptionMap.find(arg) == m_OptionMap.end())
      {
      if(failOnUnknownTrailingParameters)
        {
        // Unknown argument found
        cerr << "Unrecognized command line option '" << arg << "'" << endl;
        return false;
        }
      else return true;
      }

    // Check if the number of parameters is correct
    int nParameters = m_OptionMap[arg].NumberOfParameters;
    if(i+nParameters >= argc) 
      {
      // Too few parameters
      cerr << "Too few parameters to command line option '" << arg 
        << "'" << endl;
      return false;
      }

    // Tell the result that the option has been encountered
    outResult.AddOption(m_OptionMap[arg].CommonName,nParameters);

    // Pass in the parameters
    for(int j=0;j<nParameters;j++,i++)
      outResult.AddParameter(m_OptionMap[arg].CommonName,string(argv[i+1]));
    
    }

  // Everything is good
  return true;
}



bool 
CommandLineArgumentParseResult
::IsOptionPresent(const char *option)
{
  return (m_OptionMap.find(string(option)) != m_OptionMap.end());
}

const char * 
CommandLineArgumentParseResult
::GetOptionParameter(char *option, unsigned int number)
{
  assert(IsOptionPresent(option));
  assert(number < m_OptionMap[string(option)].size());

  return m_OptionMap[string(option)][number].c_str();
}

void  
CommandLineArgumentParseResult
::Clear()
{
  m_OptionMap.clear();
}

void  
CommandLineArgumentParseResult
::AddOption(const std::string &option, int nParms)
{
  ParameterArrayType pat;
  pat.reserve(nParms);
  m_OptionMap[option] = pat;
}

void  
CommandLineArgumentParseResult
::AddParameter(const std::string &option, const std::string &parameter)
{
  m_OptionMap[option].push_back(parameter);  
}

