//------------------------------------------------------------------------------
// Author: Pavel Karneliuk
// Description: Struct represented tcp session.
// Copyright (c) 2013 EPAM Systems. All Rights Reserved.
//------------------------------------------------------------------------------
#include "session.h"
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
namespace NST
{
namespace auxiliary
{

std::size_t Session::Hash::operator()(Session const &s) const
{
    std::size_t key(0);
    key += s.port[0] + s.port[1];
    if(s.ip_type == Session::v4)
    {
        key += s.ip.v4.addr[0] + s.ip.v4.addr[1];
    }
    else
    {
        for(int i = 0; i < 16; ++i)
        {
            key += s.ip.v6.addr[0][i] + s.ip.v6.addr[1][i];
        }
    }

    if(s.type == Session::UDP)
    {
        key = ~key;
    }
    return key;
}

bool Session::operator==(const Session& obj) const
{
    if((ip_type != obj.ip_type) || (type != obj.type))
        return false;
    if((port[0] != obj.port[0]) || (port[1] != obj.port[1]))
        return false;
    switch(ip_type)
    {
        case Session::v4: 
        {
            if((ip.v4.addr[0] != obj.ip.v4.addr[0]) || (ip.v4.addr[1] != obj.ip.v4.addr[1]))
                return false;
        }
        break;
        case Session::v6:
        {
            for(int i = 0; i < 16; ++i)
            {
                if((ip.v6.addr[0][i] != obj.ip.v6.addr[0][i]) || (ip.v6.addr[1][i] != obj.ip.v6.addr[1][i]))
                    return false;
            }
        }
        break;
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const Session& session)
{
    out << Session::session_addr(Session::Source, session) << " --> " << Session::session_addr(Session::Destination, session);
    switch(session.type)
    {
        case Session::TCP:
            out << " (TCP)";
            break;
        case Session::UDP:
            out << " (UPD)";
            break;
    }
    return out;
}

std::string Session::session_addr(Session::Direction dir, const Session& session)
{
    std::stringstream s(std::ios_base::out);
    switch(session.ip_type)
    {
        case Session::v4:
            s << ipv4_string(session.ip.v4.addr[dir]);
            break;
        case Session::v6:
            s << "(IPv6 currently not supported)";
            break;
    }
    s << ":" << session.port[dir];
    return s.str();
}

std::string Session::ipv4_string(const uint32_t ip)
{
    std::stringstream address(std::ios_base::out);
    address << ((ip >> 24) & 0xFF);
    address << '.';
    address << ((ip >> 16) & 0xFF);
    address << '.';
    address << ((ip >> 8) & 0xFF);
    address << '.';
    address << ((ip >> 0) & 0xFF);
    return address.str();
}

} // namespace auxiliary
} // namespace NST
//------------------------------------------------------------------------------

