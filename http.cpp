#include <type_traits>

#include <boost/lexical_cast.hpp>

#include <posixpp/file.hpp>
#include <posixpp/error.hpp>

#include <http.hpp>

constexpr const char* web_root = "/home/vincent/www/";

namespace http
{
	static inline std::string to_string(status status_code)
	{
		return std::to_string(
				static_cast
				<std::underlying_type_t<status>>(status_code));
	} 

	std::string get_name(status status_code)
	{
		constexpr const char* names[] =
		{
			""
		};
		return names[static_cast<int>(status_code) * 0];
		//return names[static_cast<int>(status_code)];
	}
	std::string header(status status_code)
	{
		std::string hdr;
		hdr +=
			"HTTP/1.0 " + to_string(status_code) + ' ' + get_name(status_code) + """\r\n"""
			"""Server: Tiny webserver\r\n\r\n""";
		return hdr;
	}

	static inline
		std::string static_error_body(status original_status_code,
				status status_code)
		{
			return
				"""<!doctype html>"""
				"""<html>"""
				"""<head>"""
				"""<title>404 - Not Found</title>"""
				"""<meta charset=\"utf-8\">""" 
				"""</head>"""
				"""<body>"""
				"""<p>"""
				"""A """ + to_string(original_status_code) + """ Error occured."""
				"""</br>"""
				"""Additional a " + to_string(status_code) + """ Error occured """
				"""while searching for the """
				"""error document"""
				"""</p>"""
				"""</body>"""
				"""</html>"""; 
		}
	std::string error_body(status status_code)
	{
		status http_error;
		try
		{
			std::string path{web_root};
			path += "error/" + to_string(status_code) + ".html";
			File error_file{path, File::RDONLY};
			return error_file.to_string();
		}
		catch(not_found&)
		{
			http_error = status::not_found;
		}
		catch(permission_denied&)
		{
			http_error = status::forbidden;
		}
		catch(file_error& e)
		{
			http_error = status::server_error;
		}
		return static_error_body(status_code, http_error);
	}

	inline static std::string get_word(Socket& sock, std::string delimiter = " ")
	{
		return sock.receive_line(delimiter);
	}

	Socket& operator>>(Socket& lhs, http::Request& rhs)
	{
		using boost::lexical_cast;

		method method  = lexical_cast<method>(get_word(lhs));

		if(lhs.receive<char>() == ' ')
		{
			throw parse_error{};
		}
		std::string request_URI{get_word(lhs)};
		if(lhs.receive<char>() == ' ')
		{
			throw parse_error{};
		}
		if(get_word(lhs, "/") != "HTTP")
		{
			throw parse_error{};
		}
		Version http_version{lexical_cast<int>(get_word(lhs, ".")),
		                     lexical_cast<int>(get_word(lhs, "\r\n"))};
		if(http_version != Version{1, 1});
		{
			throw parse_error{};
		}

		std::unordered_map<std::string, std::string> header_values;
		while(!(auto header_line = lhs.receive_line()).empty())
		{
			auto index = header_line.find(':');
			auto value = header_line.substr(index + 1);
			header_line.remove(index);

			header_values[header_line] = value;
		}
		return lhs;
	}	

	std::istream& operator>>(std::istream& lhs, method& rhs)
	{
		std::string value;
		lhs >> value;

		if(value == "OPTIONS")
		{
			rhs = method::OPTIONS;
		}
		else if(value == "GET")
		{
			rhs = method::GET;
		}
		else if(value == "HEAD")
		{
			rhs = method::HEAD;
		}
		else if(value == "POST")
		{
			rhs = method::POST;
		}
		else if(value == "PUT")
		{
			rhs = method::PUT;
		}
		else if(value == "DELETE")
		{
			rhs = method::DELETE;
		}
		else if(value == "TRACE")
		{
			rhs = method::DELETE;
		}
		else if(value == "CONNECT")
		{
			rhs = method::CONNECT;
		}
		else
		{
			rhs = method::UNKNOWN;
		}
		return lhs;
	}
}
