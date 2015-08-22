#ifndef TINYWEB_CXX_HTTP_HPP_INCLUDE
#define TINYWEB_CXX_HTTP_HPP_INCLUDE

#include <string>
#include <posixpp/socket.hpp>

#include "version.hpp"

namespace http
{
  // TODO: This whole section is a big great hassle.
  // First of all it must be discussed whether the nonstandard responses
  // should be implemented. For this purpose the use and acceptance of these
  // codes must be checked so a decision can be made. Although I would prefer
  // to only implement the Codes that are defined in the HTTP Specification,
  // if the code is needed for every-day use cases, its better if we implement
  // it.

  // Several other issues/wishes are:
  // TODO: Is there any possibility to use the [Deprecated] keyword for a
  // single enum value?
  //
  // TODO: Some identifiers are too long and verbose, so it would be
  // preferable to give them short descriptive names. Maybe it is the best
  // approach to just use the verbose names and introduce shorter names
  // through typedefs. The verbose names are at least easier to remember and 
  // can be used by people who only know the corresponding HTTP Return Code.
  //
  // TODO: Currently a enum class is used so there isn't any possibility to
  // give a function, that expects a HTTP status code, an int.
  // I think a constexpr ctr that takes one int should solve this problem.
  // I think it would be best practice to make the ctr explicit, but some
  // status codes are so well known(404 and alike) that the explicit doesn't
  // have to be used.
  //
  // TODO: Check the actual drafts and not just Wikipedia for the codes and
  // their semantic meanings.

  enum class status
  {
    //1xx Informational
    cont = 100,
    switch_protos = 101,
    processing = 102,

    //2xx Success
    ok = 200,
    created = 201,
    accepted = 202,
    non_authoritative_information = 203,
    no_content = 204,
    reset_content = 205,
    partial_content = 206,
    multi_status = 207,
    already_reported = 208,
    IM_used = 226,

    //3xx Redirection
    multiple_choices = 300,
    moved_permanently = 301,
    found = 302,
    see_other = 303,
    not_modified = 304,
    use_proxy = 305,
    switch_proxy = 306, // NO LONGER USED !!
    temporary_redirect = 307,
    permanent_redirect = 308,

    //4xx Client Errors
    bad_request = 400,
    unauthorized = 401,
    payment_required = 402, // Reserved for future use
    forbidden = 403,
    not_found = 404,
    method_not_allowed = 405,
    not_acceptable = 406,
    proxy_authentication_required = 407,
    request_timeout = 408,
    conflict = 409,
    gone = 410,
    length_required = 411,
    precondition_failed = 412,
    payload_too_large = 413,
    request_uri_too_long = 414,
    unsupported_media_type = 415,
    requested_range_not_satisfiable = 416,
    expectation_failed = 417,
    im_a_teapot = 418, //Should this be the default?
    authentication_timeout = 419,
    //method_failure = 420,
    //enhance_your_calm = 420,

    //5xx Server Error
    server_error = 500,
    not_implemented = 501,
    bad_gateway = 502,
    service_unavailable = 503,
    gateway_timeout = 504,
    http_version_not_supported = 505,
    variant_also_negotiates = 506,
    insufficient_storage = 507,
    loop_detected = 508,

    not_extended = 510,
  };

  enum class method
  {
    UNKNOWN,
    OPTIONS,
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    TRACE,
    CONNECT
  };

  inline std::ostream& operator<<(std::ostream& lhs, method rhs)
    {
        static const char* names[] = 
        {
            "UNKNOWN",
            "OPTIONS",
            "GET",
            "HEAD",
            "POST",
            "PUT",
            "DELETE",
            "TRACE",
            "CONNECT"
      };
        lhs <<  names[static_cast<int>(rhs)];
        return lhs;
}

  std::string header(status);
  std::string error_body(status);

  class Request
  {
    public:
        Request() = default;
        Request(const std::string);

        enum method method() { return method_;}
        void set_method(enum method m) { method_ = m;}

        const std::string& URI() { return URI_;}
        void  set_URI(const std::string new_URI) { URI_ = new_URI;}
     private:
        enum method method_;
        std::string URI_;
  };

  class Response
  {
    public:
        enum status status;
        void set_body(const std::string&);
        void set_body(const std::string&&);
  };

    struct parse_error : std::invalid_argument
    {
        public:
            parse_error() : std::invalid_argument{"parse error"}
            {
            };
            virtual const char* what() const noexcept override { return "parse error";}
    };

    Socket& operator<<(Socket&, const Response&);
    Socket& operator>>(Socket&, Request&);
}
#endif /* TINYWEB_CXX_HTTP_HPP_INCLUDE */
