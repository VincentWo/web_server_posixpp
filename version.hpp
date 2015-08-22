#ifndef POSIXPP_WEB_SERVER_VERSION_HPP_INCLUDE
#define POSIXPP_WEB_SERVER_VERSION_HPP_INCLUDE
class Version
{
    public:
        constexpr Version(int first_digit = 0, int second_digit = 0) noexcept : first_{first_digit}, second_{second_digit} 
        {};
        constexpr int first()  const noexcept { return first_; };
        constexpr int second() const noexcept { return second_;}; 

    private:
        int first_;
        int second_;
};

constexpr bool operator==(const Version& lhs, const Version& rhs)
{
    return lhs.first() == rhs.first() ? lhs.second() == rhs.second() : false;
}
constexpr bool operator<(const Version& lhs, const Version& rhs)
{
    return lhs.first() == rhs.first() ? lhs.second() < rhs.second()
                                      : lhs.first() < rhs.first();
}
constexpr bool operator>(const Version& lhs, const Version& rhs)
{
    return lhs.first() == rhs.first() ? lhs.second() < rhs.second()
                                      : lhs.first() < rhs.first();
}
constexpr bool operator!=(const Version& lhs, const Version& rhs)
{
    return !(lhs == rhs);
}
constexpr bool operator<=(const Version& lhs, const Version& rhs)
{
    return !(lhs > rhs);
}
constexpr bool operator>=(const Version& lhs, const Version& rhs)
{
    return !(lhs < rhs);
}
#endif
