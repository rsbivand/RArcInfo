/**********************************************************************
 * $Id: cpl_dir.c,v 1.1.1.1 2001/06/27 20:10:53 vir Exp $
 *
 * Name:     cpl_dir.cpp
 * Project:  CPL - Common Portability Library
 * Purpose:  Directory manipulation.
 * Author:   Daniel Morissette, danmo@videotron.ca
 *
 **********************************************************************
 * Copyright (c) 1998, Daniel Morissette
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************
 *
 * $Log: cpl_dir.c,v $
 * Revision 1.1.1.1  2001/06/27 20:10:53  vir
 * Initial release (0.1) under the cvs tree at Sourceforge.
 *
 * Revision 1.1.1.1  2001/06/27 20:04:10  vir
 * Initial release (0.1) under the CVS tree.
 *
 * Revision 1.1  1999/08/27 14:09:55  daniel
 * Update CPL files
 *
 * Revision 1.2  1999/05/20 02:54:38  warmerda
 * Added API documentation
 *
 * Revision 1.1  1999/02/25 04:52:00  danmo
 * *** empty log message ***
 *
 **********************************************************************/

#include "cpl_conv.h"
#include "cpl_string.h"


#ifdef _WIN32

/*=====================================================================
                   WIN32 / MSVC++ implementation
 *====================================================================*/

#include <io.h>

/**********************************************************************
 *                          CPLReadDir()
 *
 * Return a stringlist with the list of files in a directory.
 * The returned stringlist should be freed with CSLDestroy().
 *
 * Returns NULL if an error happened or if the directory could not
 * be read.
 **********************************************************************/

/**
 * Read names in a directory.
 *
 * This function abstracts access to directory contains.  It returns a
 * list of strings containing the names of files, and directories in this
 * directory.  The resulting string list becomes the responsibility of the
 * application and should be freed with CSLDestroy() when no longer needed.
 *
 * Note that no error is issued via CPLError() if the directory path is
 * invalid, though NULL is returned.
 *
 * @param pszPath the relative, or absolute path of a directory to read.
 * @return The list of entries in the directory, or NULL if the directory
 * doesn't exist.
 */

char **CPLReadDir(const char *pszPath)
{
    struct _finddata_t c_file;
    long    hFile;
    char    *pszFileSpec, **papszDir = NULL;

    if (strlen(pszPath) == 0)
        pszPath = ".";

    pszFileSpec = CPLStrdup(CPLSPrintf("%s\\*.*", pszPath));

    if ( (hFile = _findfirst( pszFileSpec, &c_file )) != -1L )
    {
        do
        {
            papszDir = CSLAddString(papszDir, c_file.name);
        } while( _findnext( hFile, &c_file ) == 0 );

        _findclose( hFile );
    }
    else
    {
        /* Should we generate an error???  
         * For now we'll just return NULL (at the end of the function)
         */
    }

    CPLFree(pszFileSpec);

    return papszDir;
}

#else

/*=====================================================================
                      POSIX (Unix) implementation
 *====================================================================*/

#include <sys/types.h>
#include <dirent.h>

/**********************************************************************
 *                          CPLReadDir()
 *
 * Return a stringlist with the list of files in a directory.
 * The returned stringlist should be freed with CSLDestroy().
 *
 * Returns NULL if an error happened or if the directory could not
 * be read.
 **********************************************************************/
char **CPLReadDir(const char *pszPath)
{
    DIR           *hDir;
    struct dirent *psDirEntry;
    char          **papszDir = NULL;

    if (strlen(pszPath) == 0)
        pszPath = ".";

    if ( (hDir = opendir(pszPath)) != NULL )
    {
        while( (psDirEntry = readdir(hDir)) != NULL )
        {
            papszDir = CSLAddString(papszDir, psDirEntry->d_name);
        }

        closedir( hDir );
    }
    else
    {
        /* Should we generate an error???  
         * For now we'll just return NULL (at the end of the function)
         */
    }

    return papszDir;
}

#endif
