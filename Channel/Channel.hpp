#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iomanip>
# include <iostream>
# include <map>
# include <set>
# include "../User/User.hpp"

class Channel
{
    public:

        Channel(User &user, std::string const &name);
        Channel(User &user, std::string const &name, std::string const &pass);
        ~Channel();
        std::string const &getName() const;
        std::string const &getPass() const;
        std::set<int> const &getUsers() const;
        User &getAdmin() const;
        void addUser(User *user);
        void removeUser(int fd);
        

    private:

        std::string     _name;
        std::string     _pass;
        User           &_admin;
        std::set<int>   _users;
    
};

#endif