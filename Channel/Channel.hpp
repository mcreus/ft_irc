#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <iomanip>
# include <iostream>

class Channel
{
    public:

        Channel();
        ~Channel();
        std::string const &getName() const;

    private:

        std::string _name;
};

#endif