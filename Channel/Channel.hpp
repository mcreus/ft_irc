#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iomanip>
# include <iostream>
# include <map>
# include <list>
# include "../User/User.hpp"

class Channel
{
    public:

        Channel(User &user, std::string const &name);
        Channel(User &user, std::string const &name, std::string const &pass);
        ~Channel();
        std::string const &getName() const;
        std::string const &getPass() const;
        void    createChannel(std::string const &name);
        void    joinChannel(std::string const &name);

    private:

        int                                  _nbOfChannel;
        std::string                          _name;
        std::string                          _pass;
        User                                 &_admin;
        std::map<std::string, User*>         _userInCh;
    
};

#endif