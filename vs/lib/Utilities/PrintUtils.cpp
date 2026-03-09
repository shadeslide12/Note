#include "Utilities/PrintUtils.h"
#ifdef HAVE_MPI
#include "mpi.h"
#endif
namespace Utilities
{
    std::ostream *printstream = &std::cout;

    void set_print_stream(std::ostream &o)
    {
        printstream = &o;
    }

    void print(const char *msg,
               bool if_root, bool time_stamp)
    {
        std::stringstream baseMsg;
        int rank = 0;
#ifdef HAVE_MPI
        int flag = 0;
        MPI_Initialized(&flag);
        if (if_root && flag)
        {
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        }
#endif
        if (time_stamp)
        {
            std::time_t result = std::time(nullptr);
            std::string timestr(std::asctime(std::localtime(&result)));
            timestr.resize(timestr.size() - 1);
            baseMsg << "[" << timestr << "] " << msg;
        }
        else
            baseMsg << msg;

        if (!rank)
        {
            (*printstream) << baseMsg.str() << std::endl;
        }
    }
} // namespace Utilities
