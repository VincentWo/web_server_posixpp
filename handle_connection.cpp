#include <iostream>

#include <posixpp/file.hpp>
#include <posixpp/error.hpp>

#include "http.hpp"

using namespace http;

File get_resource(const std::string&);

void handle_connection(Connection& conn, std::ostream& log = std::cerr)
{
	http::Request request{};
	conn.first >> request;
	http::Response response{};

	if(request.method() != method::GET && request.method() != method::HEAD)
	{
		std::cerr << "ERROR UNSUPPORTED METHOD USED\n";
		return;
	}

	try
	{
		File resource = get_resource(request.URI());
		response.status = status::ok;
		if(request.method() != method::HEAD)
		{
			response.set_body(resource.to_string());
		}
	}
	catch(not_found&)
	{
		response.status = status::not_found;
		response.set_body(error_body(status::not_found));
	}
	catch(permission_denied&)
	{
		response.status = status::forbidden;
		response.set_body(error_body(status::not_found));
	}
	catch(std::exception& e)
	{
		response.status = status::server_error;
		response.set_body(error_body(status::server_error));

		log << "Getting the resource failed with an unknown exception.\n";
		log << ".what(): " << e.what() << '\n';

		conn.first << response;
		throw;
	}
	conn.first << response;
}
