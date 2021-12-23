#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <list>
#include <sys/stat.h>

int main()
{

    std::string freespaceHostString, usedpaceDevString, deviceName, junk, mountLocation, space, rootDevName;
    int freeSpaceHost, usedSpaceDev;
    char scaleHost, scaleDev;
    std::vector<std::string> systemDevices;
    std::list<std::string> connectedDevices;

    {
        struct stat st;
        if (stat("/usr/install", &st) == 0)
        {
            if (st.st_mode & S_IFDIR != 0)
            {
                std::cout << "Directory Exist." << std::endl;
            }
        }
        else
        {
            system("sudo mkdir -p /usr/install");
            std::cout << "Created Directory " << std::endl;
        }
    }

    std::cout << "Make sure no external devices are connected before analysis. Analysis starts in 5 sec." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(5));

    system("df -H > connectedDevices.dat");

    {
        std::ifstream infile("connectedDevices.dat");

        bool inHeader = true;

        while (infile >> deviceName >> junk >> junk >> space >> junk >> mountLocation)
        {
            if(!inHeader)
            {
                systemDevices.push_back(deviceName);

                if (mountLocation.compare("/") == 0)
                {
                    freespaceHostString = space;
                }
            }
            else
            {
                inHeader = false;
                infile >> junk;
            }
        }

        infile.close();
    }

    std::cout << "Analisys Over, you can now connect your device." << std::endl;

    freeSpaceHost = stoi(freespaceHostString);

    for (char character : freespaceHostString)
    {
        if (character > 58)
        {
            scaleHost = character;
            break;
        }
    }

    std::cout << "Available space in root device : " << freeSpaceHost << " | scale : " << scaleHost << std::endl;

    switch (scaleHost)
    {
    case 'G':
        freeSpaceHost = freeSpaceHost * (1024 * 1024);
        break;
    case 'M':
        freeSpaceHost = freeSpaceHost * 1024;
        break;
    default:
        break;
    }
    do
    {
        bool exist = false;

        system("df -H > connectedDevices.dat");

        std::ifstream infile("connectedDevices.dat");

        bool inHeader = true;

        while (infile >> deviceName >> junk >> usedpaceDevString >> junk >> junk >> junk)
        {
            if (!inHeader)
            {
                for (const auto name : systemDevices)
                {
                    if (deviceName.compare(name) == 0)
                    {
                        exist = true;
                        break;
                    }
                }

                if (!exist)
                {
                    for (const auto name : connectedDevices)
                    {
                        if (deviceName.compare(name) == 0)
                        {
                            exist = true;
                            break;
                        }
                    }

                    if (!exist)
                    {
                        usedSpaceDev = stoi(usedpaceDevString);

                        for (char character : usedpaceDevString)
                        {
                            if (character > 58)
                            {
                                scaleDev = character;
                                break;
                            }
                        }

                        std::cout << "New Device Detected > Name : " << deviceName
                                  << " | Data Size : " << usedpaceDevString << ". Do you want to copy Data(y/n) :  " << std::flush;

                        if ((char)std::cin.get() == 'y')
                        {
                            switch (scaleDev)
                            {
                            case 'G':
                                usedSpaceDev = usedSpaceDev * (1024 * 1024);
                                break;
                            case 'M':
                                usedSpaceDev = usedSpaceDev * 1024;
                                break;
                            default:
                                break;
                            }

                            if (usedSpaceDev < freeSpaceHost)
                            {
                                std::string command("sudo cp -r ");
                                command = command + deviceName + "* /usr/install";
                                system(command.c_str());
                                connectedDevices.push_back(deviceName);
                            }
                            else
                                std::cout << "No space Available" << std::endl;
                        }
                        else if ((char)std::cin.get() == 'n')
                        {
                            connectedDevices.push_back(deviceName);
                        }
                        else
                            continue;
                    }
                }
            }

            else
            {
                inHeader = false;
                infile >> junk;
            }
        }

        infile.close();

        for(auto it = connectedDevices.begin(); it != connectedDevices.end(); it++)
        {
            bool exist = false;
            inHeader = true;
            std::ifstream infile("connectedDevices.dat");
            while(infile >> deviceName >> junk >> usedpaceDevString >> junk >> junk >> junk)
            {
                if (!inHeader)
                {
                    if (it->compare(deviceName) == 0)
                    {
                        exist = true;
                        break;
                    }
                }
                else
                {
                    inHeader = false;
                    infile >> junk;
                }
            }

            if(!exist)
                it = connectedDevices.erase(it);

            infile.close();
        }


    } while (true);
}