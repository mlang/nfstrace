//------------------------------------------------------------------------------
// Author: Dzianis Huznou
// Description: Analyzers storage. 
// Copyright (c) 2013 EPAM Systems. All Rights Reserved.
//------------------------------------------------------------------------------
#ifndef ANALYZERS_H
#define ANALYZERS_H
//------------------------------------------------------------------------------
#include <list>
#include <vector>

#include "../controller/running_status.h"
#include "../filter/nfs/nfs_operation.h"
#include "../filter/nfs/nfs_struct.h"
#include "base_analyzer.h"
//------------------------------------------------------------------------------
using namespace NST::filter::NFS3;
//------------------------------------------------------------------------------
namespace NST
{
namespace analyzer
{

class Analyzers
{
    typedef std::list<BaseAnalyzer*> Storage;

    typedef bool (BaseAnalyzer::*Method)(const BaseAnalyzer::Session& session, const NFSOperation& operation);
public:
    Analyzers()
    {
        methods.resize(Proc::num);

        methods[Proc::NFS_NULL]     = &BaseAnalyzer::call_null;
        methods[Proc::GETATTR]      = &BaseAnalyzer::call_getattr;
        methods[Proc::SETATTR]      = &BaseAnalyzer::call_setattr;
        methods[Proc::LOOKUP]       = &BaseAnalyzer::call_lookup;
        methods[Proc::ACCESS]       = &BaseAnalyzer::call_access;
        methods[Proc::READLINK]     = &BaseAnalyzer::call_readlink;
        methods[Proc::READ]         = &BaseAnalyzer::call_read;
        methods[Proc::WRITE]        = &BaseAnalyzer::call_write;
        methods[Proc::CREATE]       = &BaseAnalyzer::call_create;
        methods[Proc::MKDIR]        = &BaseAnalyzer::call_mkdir;
        methods[Proc::SYMLINK]      = &BaseAnalyzer::call_symlink;
        methods[Proc::MKNOD]        = &BaseAnalyzer::call_mknod;
        methods[Proc::REMOVE]       = &BaseAnalyzer::call_remove;
        methods[Proc::RMDIR]        = &BaseAnalyzer::call_rmdir;
        methods[Proc::RENAME]       = &BaseAnalyzer::call_rename;
        methods[Proc::LINK]         = &BaseAnalyzer::call_link;
        methods[Proc::READDIR]      = &BaseAnalyzer::call_readdir;
        methods[Proc::READDIRPLUS]  = &BaseAnalyzer::call_readdirplus;
        methods[Proc::FSSTAT]       = &BaseAnalyzer::call_fsstat;
        methods[Proc::FSINFO]       = &BaseAnalyzer::call_fsinfo;
        methods[Proc::PATHCONF]     = &BaseAnalyzer::call_pathconf;
        methods[Proc::COMMIT]       = &BaseAnalyzer::call_commit;
    }
    ~Analyzers()
    {
        Storage::iterator i = analyzers.begin();
        Storage::iterator end = analyzers.end();
        for(; i != end; ++i)
        {
            delete *i;
        }
    }

    void add(BaseAnalyzer* analyzer)
    {
        analyzers.push_back(analyzer);
    }

    bool call(const BaseAnalyzer::Session& session, const NFSOperation& operation)
    {
        Storage::iterator i = analyzers.begin();
        Storage::iterator end = analyzers.end();
        for(; i != end; ++i)
        {
            ((*i)->*methods[operation])(session, operation);
        }
        return true;
    }

private:
    Storage analyzers;
    std::vector<Method> methods;
};

} // namespace analyzer
} // namespace NST
//------------------------------------------------------------------------------
#endif//ANALYZERS_H
//------------------------------------------------------------------------------
