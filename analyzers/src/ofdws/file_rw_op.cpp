//------------------------------------------------------------------------------
// Author: Dzianis Huznou
// Description: Hash-table hold info about read/write operations.
// Copyright (c) 2013 EPAM Systems
//------------------------------------------------------------------------------
/*
    This file is part of Nfstrace.

    Nfstrace is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, version 2 of the License.

    Nfstrace is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Nfstrace.  If not, see <http://www.gnu.org/licenses/>.
*/
//------------------------------------------------------------------------------
#include "file_rw_op.h"
//------------------------------------------------------------------------------
uint32_t FileRWOp::block_size  {0};
uint32_t FileRWOp::bucket_size {0};

FileRWOp::~FileRWOp()
{
    Iterator i = buckets.begin();
    Iterator end = buckets.end();
    for(; i != end; ++i)
        delete [] i->second;
}

void FileRWOp::calculate(ProcEnumNFS3::NFSProcedure op, uint64_t o, uint32_t c, uint32_t)
{
    if(op == ProcEnumNFS3::READ)
        read_total += c;
    else 
        write_total += c;

    uint64_t bl_ind = o / block_size;
    uint64_t bl_end = (o + c) / block_size;
    if((o + c) % block_size)
        ++bl_end;

    uint64_t b_ind = bl_ind / bucket_size;
    uint64_t b_end = bl_end / bucket_size;
    if(bl_end % bucket_size)
        ++b_end;

    for(uint64_t bl_cur = bl_ind; b_ind < b_end; ++b_ind) 
    {
        Iterator it = buckets.find(b_ind);
        if(it == buckets.end())
        {
            Inserted ins = buckets.insert(Pair(b_ind, new RWTime[bucket_size]));
            if(ins.second == false)
                throw int();
            it = ins.first;
        }
        RWTime* bucket = it->second;

        uint64_t start = bl_cur % bucket_size;
        uint64_t end = bucket_size;
        if(bl_cur / bucket_size == b_end - 1)
        {
            end = bl_end % bucket_size;
            if(!end)
                end = bucket_size;
        }
        if(op == ProcEnumNFS3::READ)
            for(uint64_t i = start; i < end; ++i)
                bucket[i].inc_read_freq();
        else
            for(uint64_t i = start; i < end; ++i)
                bucket[i].inc_write_freq();
        bl_cur += end - start;
    }
}
//------------------------------------------------------------------------------
