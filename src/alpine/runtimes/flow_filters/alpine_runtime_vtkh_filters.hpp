//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2015-2017, Lawrence Livermore National Security, LLC.
// 
// Produced at the Lawrence Livermore National Laboratory
// 
// LLNL-CODE-716457
// 
// All rights reserved.
// 
// This file is part of Alpine. 
// 
// For details, see: http://software.llnl.gov/alpine/.
// 
// Please also read alpine/LICENSE
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the disclaimer below.
// 
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the disclaimer (as noted below) in the
//   documentation and/or other materials provided with the distribution.
// 
// * Neither the name of the LLNS/LLNL nor the names of its contributors may
//   be used to endorse or promote products derived from this software without
//   specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE LIVERMORE NATIONAL SECURITY,
// LLC, THE U.S. DEPARTMENT OF ENERGY OR CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
// DAMAGES  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
// IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


//-----------------------------------------------------------------------------
///
/// file: alpine_runtime_vtkh_filters.hpp
///
//-----------------------------------------------------------------------------

#ifndef ALPINE_RUNTIME_VTKH_FILTERS
#define ALPINE_RUNTIME_VTKH_FILTERS

#include <alpine.hpp>

#include <flow_filter.hpp>


//-----------------------------------------------------------------------------
// -- begin alpine:: --
//-----------------------------------------------------------------------------
namespace alpine
{

//-----------------------------------------------------------------------------
// -- begin alpine::runtime --
//-----------------------------------------------------------------------------
namespace runtime
{

//-----------------------------------------------------------------------------
// -- begin alpine::runtime::filters --
//-----------------------------------------------------------------------------
namespace filters
{

void write_log();
void write_entry(std::string name, double time);

//-----------------------------------------------------------------------------
///
/// VTK-H Filters
///
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class EnsureVTKH : public ::flow::Filter
{
public:
    EnsureVTKH();
   ~EnsureVTKH();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual void   execute();
};

//-----------------------------------------------------------------------------
class EnsureVTKM : public ::flow::Filter
{
public:
    EnsureVTKM();
   ~EnsureVTKM();

    virtual void   declare_interface(conduit::Node &i);
    virtual void   execute();
};

//-----------------------------------------------------------------------------
class VTKHRayTracer : public ::flow::Filter
{
public:
    VTKHRayTracer();
   ~VTKHRayTracer();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual bool   verify_params(const conduit::Node &params,
                                 conduit::Node &info);
    virtual void   execute();
};

//-----------------------------------------------------------------------------
class VTKHVolumeTracer : public ::flow::Filter
{
public:
    VTKHVolumeTracer();
   ~VTKHVolumeTracer();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual bool   verify_params(const conduit::Node &params,
                                 conduit::Node &info);
    virtual void   execute();
};

//-----------------------------------------------------------------------------
class VTKHMarchingCubes : public ::flow::Filter
{
public:
    VTKHMarchingCubes();
   ~VTKHMarchingCubes();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual bool   verify_params(const conduit::Node &params,
                                 conduit::Node &info);
    virtual void   execute();
};

//-----------------------------------------------------------------------------
class VTKHThreshold : public ::flow::Filter
{
public:
    VTKHThreshold();
   ~VTKHThreshold();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual bool   verify_params(const conduit::Node &params,
                                 conduit::Node &info);
    virtual void   execute();
};

//-----------------------------------------------------------------------------
class VTKHClip: public ::flow::Filter
{
public:
    VTKHClip();
   ~VTKHClip();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual bool   verify_params(const conduit::Node &params,
                                 conduit::Node &info);
    virtual void   execute();
};

//-----------------------------------------------------------------------------
class DefaultRender : public ::flow::Filter
{
public:
    DefaultRender();
   ~DefaultRender();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual bool   verify_params(const conduit::Node &params,
                                 conduit::Node &info);
    virtual void   execute();
};



//-----------------------------------------------------------------------------
class VTKHBounds: public ::flow::Filter
{
public:
    VTKHBounds();
   ~VTKHBounds();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual void   execute();
};

//-----------------------------------------------------------------------------
class VTKHUnionBounds: public ::flow::Filter
{
public:
    VTKHUnionBounds();
   ~VTKHUnionBounds();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual void   execute();
};



//-----------------------------------------------------------------------------
class VTKHDomainIds: public ::flow::Filter
{
public:
    VTKHDomainIds();
   ~VTKHDomainIds();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual void   execute();
};

//-----------------------------------------------------------------------------
class VTKHUnionDomainIds: public ::flow::Filter
{
public:
    VTKHUnionDomainIds();
   ~VTKHUnionDomainIds();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual void   execute();
};


//-----------------------------------------------------------------------------
class Scene: public ::flow::Filter
{
public:
    Scene();
   ~Scene();
    
    virtual void   declare_interface(conduit::Node &i);
    virtual bool   verify_params(const conduit::Node &params,
                                 conduit::Node &info);
    virtual void   execute();

private:
    static int s_image_count;
};




};
//-----------------------------------------------------------------------------
// -- end alpine::runtime::filters --
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
// -- end alpine::runtime --
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
};
//-----------------------------------------------------------------------------
// -- end alpine:: --
//-----------------------------------------------------------------------------




#endif
//-----------------------------------------------------------------------------
// -- end header ifdef guard
//-----------------------------------------------------------------------------
