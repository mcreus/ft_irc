#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iomanip>
# include <iostream>
# include <map>
# include "../User/User.hpp"

class Channel
{
    public:

        Channel();
        ~Channel();
        std::string const &getName() const;

    private:

        std::string _name;
        User    *_admin;
        std::map<std::string, User*> _userInCh;
        User    *Channel::admin();

};

#endif