#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iomanip>
# include <iostream>
# include <map>
# include <set>
# include "../User/User.hpp"

class User;

class Channel
{
    public:

        Channel(User &user, std::string const &name);
        Channel(User &user, std::string const &name, std::string const &pass);
        ~Channel();
        std::string const &getName() const;
        std::string const &getPass() const;
        std::map<int, User *> const &getUsers() const;
        User *getAdmin() const;
        void addUser(User *user);
        void removeUser(std::string target);
        void	setTopic(std::string);
        std::string	getTopic();
        

    private:

        std::string     _name;
        std::string     _pass;
        std::string	_topic;
        
        User					*admin;
        std::map<int, User *>   users;
        int						nextUserNumber;
    
};

#endif
