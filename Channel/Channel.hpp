#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iomanip>
# include <iostream>
# include <map>
# include <set>
# include "../User/User.hpp"
# include <sys/socket.h>

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
        User *getHost() const;
        void addUser(User *user);
        void removeUser(std::string target);
        void	setTopic(std::string);
        std::string	getTopic();
        void addAdmin(User *admin);
        void addInvite(User *user);
        void removeAdmin(User *admin);
        void removeInvite(User *user);
        bool isUserAdmin(User *user) const;
        bool isUserInvite(User *user) const;
        void	updateAdmin();
        void	ModeInvite(bool, int);
        void	ModeTopic(bool, int);
        void	ModeKey(bool, std::string, int);
        void	ModeLimite(bool, int, int);
        void	ModeOp(bool, std::string, int);
        bool	VerifPassword(std::string);
        bool	VerifLimite();
        bool	ChannelInvite();
        bool	ChannelTopic();

    private:

        std::string     _name;
        std::string     _pass;
        std::string	_topic;
        
        User	*host;
        std::map<int, User *>   users;
        std::set<User *>	admins;
        std::set<User *>	invite;
        int	nextUserNumber;
        int	limiteUser;
        int nb_user;
        bool	i;
        bool	t;
        bool	k;
        bool	l;
};

#endif
