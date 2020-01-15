/*  +---------------------------------------------------------------------------+
*  |                                                                           |
*  |                                                                           |
*  |                                                                           |
*  |                                                                           |
*  | Authors: Mauro Bellone - http://www.maurobellone.com                      |
*  | Released under GNU GPL License.                                           |
*  +---------------------------------------------------------------------------+ */

//modified version from here https://stackoverflow.com/questions/12978973/unknown-ouput-with-qdebugstream-and-qtextedit  

#ifndef QDEBUGSTREAM_H
#define QDEBUGSTREAM_H

#include <iostream>
#include <streambuf>
#include <string>

#include "qtextedit.h"

/** \brief QDebugStream allows to redirect cout and cerr into a QTextEdit
*
*/
class QDebugStream : public std::basic_streambuf<char>
{

public:

	/** \brief Constructor
	*
	*/
	 explicit QDebugStream(std::ostream &stream, QTextEdit* text_edit) : m_stream(stream)
	 {
	  log_window = text_edit;
	  m_old_buf = stream.rdbuf();
	  stream.rdbuf(this);
	  to_terminal = false;
	  to_GUI = false;
	  verbose = false;
	 }

	 /** \brief Detor
	 *   
	 *   Output anything that is left
	 *
	 */
	 ~QDebugStream()
	 {
		 if (!m_string.empty()) {
			 if (verbose) 
			 {
				if (to_GUI) { 
					 log_window->append(m_string.c_str()); 
				}
				if (to_terminal) { 
					 printf("%s", m_string.c_str()); printf("\n");
				}
			 }
		 }
	  // redirect again to the old buffer (cout of cerr)
	  m_stream.rdbuf(m_old_buf);
	 }

	 /** \brief Set a flag to allow messages to be copied in the terminal
	 *	
	 *    This expose to the user the possibility of setting the redirect buffer 
	 *    to the GUI or not
	 */
	void copyOutToTerminal(bool _to_terminal) { to_terminal = _to_terminal; }

	/** \brief Set a flag to allow messages to be copied in the GUI
	*
	*    This expose to the user the possibility of setting the redirect buffer 
	*    to the GUI or not
	*/
	void redirectOutInGUI(bool _to_GUI) { to_GUI = _to_GUI; }

	void setVerbose(bool _verbose) { verbose = _verbose; }

protected:

	/** \brief Called on endline
	*
	*    Called on endline (either cerr or cout according to the setting),
	*/
	virtual int_type overflow(int_type v)
	{
		if (v == '\n')
		{
			if (verbose)
			{
				std::string toPrint = QDate::currentDate().toString().toStdString() + "  "
					+ QTime::currentTime().toString().toStdString() + " " + m_string;
				if (to_GUI) {
					log_window->append(toPrint.c_str());
				}
				if (to_terminal) {
					printf("%s", toPrint.c_str());
					printf("\n");
				}
				m_string.erase(m_string.begin(), m_string.end());
			}
		}
	  else
	   m_string += v;

	  return v;
	}

	/** \brief Called on output stream 
	*
	*    Called on output stream (either cerr or cout according to the setting),
	*    it appends the new text to the buffer until it finds a new line \n
	*   
	*/
	virtual std::streamsize xsputn(const char *p, std::streamsize n)
	{

	  m_string.append(p, p + n);
  
	  int pos = 0;
	  while (pos != std::string::npos) // npos = bad/missing length/position
	  {
	   pos = int (m_string.find('\n'));
	   if (pos != std::string::npos)
	   {
		std::string tmp(m_string.begin(), m_string.begin() + pos);
		if (to_GUI) { 
			log_window->append(tmp.c_str()); 
		}
		if (to_terminal) { 
			printf("%s", tmp.c_str()); printf("\n"); 
		}

		if (m_string.size()>1) m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
	   }
	  }

	  return n;
	 }

private:
	std::ostream &m_stream;    //<! the stream buffer 
	std::streambuf *m_old_buf; //<! the redirected stream buffer (for instance std::cout of cerr)
	std::string m_string;      //<! actual message string
	bool to_terminal;          //<! if true the output will also go to the terminal
	bool to_GUI;               //<! if true the output will also go to the GUI
	bool verbose;               //<! if true the output will also go to the GUI

	QTextEdit* log_window;     /*<! pointer to the QTextEdit object in the GUI, 
							        this must be set upon construction */
};

#endif