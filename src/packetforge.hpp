#ifndef PACKETFORGE_HPP
#define PACKETFORGE_HPP

#include "prototypes.hpp"

class PacketForge
{
    static PacketForge* forge;
    QLibrary pktDecrypt{ "PacketForge.dll" };

    //Typedefs for imported functions from our PacketDecrypt Library.
    typedef QString (*Encrypt)(QString);
    typedef QString (*Decrypt)(QString);
    typedef bool (*CanSend)(QString, QString);

    //Function Pointers for imported functions from the PacketDecrypt Library.
    Decrypt decryptPkt;

    public:
        PacketForge();
        ~PacketForge();

        static PacketForge* getInstance();

        //Wrappers for our imported functions.
        QString decryptPacket(QString packet);
};

#endif // PACKETFORGE_HPP