//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2015-2017, Lawrence Livermore National Security, LLC.
// 
// Produced at the Lawrence Livermore National Laboratory
// 
// LLNL-CODE-716457
// 
// All rights reserved.
// 
// This file is part of Ascent. 
// 
// For details, see: http://software.llnl.gov/ascent/.
// 
// Please also read ascent/LICENSE
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
/// file: t_ascent_render_3d.cpp
///
//-----------------------------------------------------------------------------


#include "gtest/gtest.h"

#include <ascent.hpp>

#include <iostream>
#include <math.h>

#include <conduit_blueprint.hpp>

#include "t_config.hpp"
#include "t_utils.hpp"




using namespace std;
using namespace conduit;
using namespace ascent;


index_t EXAMPLE_MESH_SIDE_DIM = 20;


//-----------------------------------------------------------------------------
TEST(ascent_render_3d, test_render_3d_render_default_runtime)
{
    // the ascent runtime is currently our only rendering runtime
    Node n;
    ascent::about(n);
    // only run this test if ascent was built with vtkm support
    if(n["runtimes/ascent/status"].as_string() == "disabled")
    {
        ASCENT_INFO("Ascent support disabled, skipping 3D default"
                      "Pipeline test");

        return;
    }
    
    
    //
    // Create an example mesh.
    //
    Node data, verify_info;
    conduit::blueprint::mesh::examples::braid("hexs",
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              data);
    
    EXPECT_TRUE(conduit::blueprint::mesh::verify(data,verify_info));
    verify_info.print();

    ASCENT_INFO("Testing 3D Rendering with Default Pipeline");


    string output_path = prepare_output_dir();
    string output_file = conduit::utils::join_file_path(output_path,"tout_render_3d_default_runtime");
    
    // remove old images before rendering
    remove_test_image(output_file);


    //
    // Create the actions.
    //

    conduit::Node scenes;
    scenes["s1/plots/p1/type"]         = "pseudocolor";
    scenes["s1/plots/p1/params/field"] = "braid";
    scenes["s1/image_prefix"] = output_file;
 
 
    conduit::Node actions;
    conduit::Node &add_plots = actions.append();
    add_plots["action"] = "add_scenes";
    add_plots["scenes"] = scenes;
    conduit::Node &execute  = actions.append();
    execute["action"] = "execute";
    
    //
    // Run Ascent
    //
    
    Ascent ascent;

    Node ascent_opts;
    ascent_opts["runtime/type"] = "ascent";
    ascent.open(ascent_opts);
    ascent.publish(data);
    ascent.execute(actions);
    ascent.close();
    
    // check that we created an image
    EXPECT_TRUE(check_test_image(output_file));
}


//-----------------------------------------------------------------------------
TEST(ascent_render_3d, test_render_3d_render_ascent_serial_backend)
{
    
    Node n;
    ascent::about(n);
    // only run this test if ascent was built with vtkm support
    if(n["runtimes/ascent/status"].as_string() == "disabled")
    {
        ASCENT_INFO("Ascent support disabled, skipping 3D serial test");
        return;
    }
    
    ASCENT_INFO("Testing 3D Rendering with Ascent runtime using Serial Backend");
    
    //
    // Create an example mesh.
    //
    Node data, verify_info;
    conduit::blueprint::mesh::examples::braid("hexs",
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              data);
    
    EXPECT_TRUE(conduit::blueprint::mesh::verify(data,verify_info));
    verify_info.print();

    string output_path = prepare_output_dir();
    string output_file = conduit::utils::join_file_path(output_path, "tout_render_3d_ascent_serial_backend");

    // remove old images before rendering
    remove_test_image(output_file);

    //
    // Create the actions.
    //

    conduit::Node scenes;
    scenes["s1/plots/p1/type"]         = "pseudocolor";
    scenes["s1/plots/p1/params/field"] = "braid";
    scenes["s1/image_prefix"] = output_file;
 
 
    conduit::Node actions;
    conduit::Node &add_plots = actions.append();
    add_plots["action"] = "add_scenes";
    add_plots["scenes"] = scenes;
    conduit::Node &execute  = actions.append();
    execute["action"] = "execute";
    actions.print();
    
    //
    // Run Ascent
    //
    
    Ascent ascent;

    Node ascent_opts;
    ascent_opts["runtime/type"] = "ascent";
    ascent_opts["runtime/backend"] = "serial";
    ascent.open(ascent_opts);
    ascent.publish(data);
    ascent.execute(actions);
    ascent.close();
    
    // check that we created an image
    EXPECT_TRUE(check_test_image(output_file));
}



//-----------------------------------------------------------------------------
TEST(ascent_render_3d, test_render_3d_render_acsent_tbb_backend)
{
    
    Node n;
    ascent::about(n);
    // only run this test if ascent was built with vtkm support
    if(n["runtimes/ascent/status"].as_string() == "disabled")
    {
        ASCENT_INFO("Ascent support disabled, skipping 3D Ascent-tbb test");
        return;
    }
    
    if(n["runtimes/ascent/backends/tbb"].as_string() != "enabled")
    {
        ASCENT_INFO("Ascent TBB support disabled, skipping 3D Ascent-tbb test");
        return;
    }
    
    ASCENT_INFO("Testing 3D Rendering with Ascent using TBB Backend");
    
    //
    // Create an example mesh.
    //
    Node data, verify_info;
    conduit::blueprint::mesh::examples::braid("hexs",
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              data);
    
    EXPECT_TRUE(conduit::blueprint::mesh::verify(data,verify_info));
    verify_info.print();


    string output_path = prepare_output_dir();
    string output_file = conduit::utils::join_file_path(output_path, "tout_render_3d_ascent_tbb_backend");

    // remove old images before rendering
    remove_test_image(output_file);


    //
    // Create the actions.
    //

    conduit::Node scenes;
    scenes["s1/plots/p1/type"]         = "pseudocolor";
    scenes["s1/plots/p1/params/field"] = "braid";
    scenes["s1/image_prefix"] = output_file;
 
    conduit::Node actions;
    conduit::Node &add_plots = actions.append();
    add_plots["action"] = "add_scenes";
    add_plots["scenes"] = scenes;
    conduit::Node &execute  = actions.append();
    execute["action"] = "execute";
    actions.print();
    
    //
    // Run Ascent
    //
    
    Ascent ascent;

    Node ascent_opts;
    ascent_opts["runtime/type"] = "ascent";
    ascent_opts["runtime/backend"] = "tbb";
    ascent.open(ascent_opts);
    ascent.publish(data);
    ascent.execute(actions);
    ascent.close();
    
    // check that we created an image
    EXPECT_TRUE(check_test_image(output_file));
}


//-----------------------------------------------------------------------------
TEST(ascent_render_3d, test_3d_render_ascent_runtime_cuda_backend)
{
    
    Node n;
    ascent::about(n);
    // only run this test if ascent was built with vtkm support
    if(n["runtimes/ascent/status"].as_string() == "disabled")
    {
        ASCENT_INFO("Ascent support disabled, skipping 3D Ascent-cuda test");
        return;
    }
    
    if(n["runtimes/ascent/backends/cuda"].as_string() != "enabled")
    {
        ASCENT_INFO("Ascent CUDA support disabled, skipping 3D Ascent-cuda test");
        return;
    }
    
    ASCENT_INFO("Testing 3D Rendering with Ascent runtime  using CUDA Backend");
    
    //
    // Create an example mesh.
    //
    Node data, verify_info;
    conduit::blueprint::mesh::examples::braid("hexs",
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              data);
    
    EXPECT_TRUE(conduit::blueprint::mesh::verify(data,verify_info));
    verify_info.print();

    
    string output_path = prepare_output_dir();
    string output_file = conduit::utils::join_file_path(output_path, "tout_render_3d_vtkm_cuda_backend");

    // remove old images before rendering
    remove_test_image(output_file);

    //
    // Create the actions.
    //

    conduit::Node scenes;
    scenes["s1/plots/p1/type"]         = "pseudocolor";
    scenes["s1/plots/p1/params/field"] = "braid";
    scenes["s1/image_prefix"] = output_file;
 
    conduit::Node actions;
    conduit::Node &add_plots = actions.append();
    add_plots["action"] = "add_scenes";
    add_plots["scenes"] = scenes;
    conduit::Node &execute  = actions.append();
    execute["action"] = "execute";
    
    //
    // Run Ascent
    //
    
    Ascent ascent;

    Node ascent_opts;
    ascent_opts["runtime/type"] = "ascent";
    ascent_opts["runtime/backend"] = "cuda";
    ascent.open(ascent_opts);
    ascent.publish(data);
    ascent.execute(actions);
    ascent.close();
    
    // check that we created an image
    EXPECT_TRUE(check_test_image(output_file));
}

//-----------------------------------------------------------------------------
TEST(ascent_render_3d, test_render_3d_multi_render)
{
    // the ascent runtime is currently our only rendering runtime
    Node n;
    ascent::about(n);
    // only run this test if ascent was built with vtkm support
    if(n["runtimes/ascent/status"].as_string() == "disabled")
    {
        ASCENT_INFO("Ascent support disabled, skipping 3D default"
                      "Pipeline test");

        return;
    }
    
    
    //
    // Create an example mesh.
    //
    Node data, verify_info;
    conduit::blueprint::mesh::examples::braid("hexs",
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              EXAMPLE_MESH_SIDE_DIM,
                                              data);
    
    EXPECT_TRUE(conduit::blueprint::mesh::verify(data,verify_info));
    verify_info.print();

    ASCENT_INFO("Testing 3D Rendering with Default Pipeline");

    string output_path = prepare_output_dir();
    string image_name0 = "render_0";
    string output_file = conduit::utils::join_file_path(output_path,image_name0);
    
    // remove old images before rendering
    remove_test_image(output_file);

    string image_name1 = "render_1";
    string output_file1 = conduit::utils::join_file_path(output_path,image_name1);
    
    // remove old images before rendering
    remove_test_image(output_file1);


    //
    // Create the actions.
    //

    conduit::Node scenes;
    scenes["s1/plots/p1/type"]         = "pseudocolor";
    scenes["s1/plots/p1/params/field"] = "braid";
    scenes["s1/image_prefix"] = output_file;
 
    scenes["s1/renders/r1/image_width"]  = 512;
    scenes["s1/renders/r1/image_height"] = 512;
    scenes["s1/renders/r1/image_name"]   = output_file;
    scenes["s1/renders/r1/color_table/name"]   = "blue";
    
    // 
    scenes["s1/renders/r2/image_width"]  = 300;
    scenes["s1/renders/r2/image_height"] = 400;
    scenes["s1/renders/r2/image_name"]   = output_file1;
    double vec3[3];
    vec3[0] = 1.; vec3[1] = 1.; vec3[2] = 1.;
    scenes["s1/renders/r2/camera/look_at"].set_float64_ptr(vec3,3);
    vec3[0] = 15.; vec3[1] = 17.; vec3[2] = 15.;
    scenes["s1/renders/r2/camera/position"].set_float64_ptr(vec3,3);
    vec3[0] = 0.; vec3[1] = -1.; vec3[2] = 0.;
    scenes["s1/renders/r2/camera/up"].set_float64_ptr(vec3,3);
    scenes["s1/renders/r2/camera/fov"] = 45.;
    scenes["s1/renders/r2/camera/xpan"] = 1.;
    scenes["s1/renders/r2/camera/ypan"] = 1.;
    scenes["s1/renders/r2/camera/zoom"] = 3.2;
    scenes["s1/renders/r2/camera/near_plane"] = 0.1;
    scenes["s1/renders/r2/camera/far_plane"] = 33.1;
     
    conduit::Node control_points;
    conduit::Node &point1 = control_points.append();
    point1["type"] = "rgb";
    point1["position"] = 0.;
    double color[3] = {1., 0., 0.};
    point1["color"].set_float64_ptr(color, 3);

    conduit::Node &point2 = control_points.append();
    point2["type"] = "rgb";
    point2["position"] = 0.5;
    color[0] = 0;
    color[1] = 1.;
    point2["color"].set_float64_ptr(color, 3);

    conduit::Node &point3 = control_points.append();
    point3["type"] = "rgb";
    point3["position"] = 1.0;
    color[1] = 0;
    color[2] = 1.;
    point3["color"].set_float64_ptr(color, 3);

    conduit::Node &point4 = control_points.append();
    point4["type"] = "alpha";
    point4["position"] = 0.;
    point4["alpha"] = 0.;

    conduit::Node &point5 = control_points.append();
    point5["type"] = "alpha";
    point5["position"] = 1.0;
    point5["alpha"] = 1.; 
    scenes["s1/renders/r2/color_table/control_points"] = control_points;

    conduit::Node actions;
    conduit::Node &add_plots = actions.append();
    add_plots["action"] = "add_scenes";
    add_plots["scenes"] = scenes;
    conduit::Node &execute  = actions.append();
    execute["action"] = "execute";
    
    //
    // Run Ascent
    //
    
    Ascent ascent;

    Node ascent_opts;
    ascent_opts["runtime/type"] = "ascent";
    ascent.open(ascent_opts);
    ascent.publish(data);
    ascent.execute(actions);
    ascent.close();
    
    // check that we created an image
    EXPECT_TRUE(check_test_image(output_file));
    // check that we created an image
    EXPECT_TRUE(check_test_image(output_file1));
}




//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    int result = 0;

    ::testing::InitGoogleTest(&argc, argv);
    
    // allow override of the data size via the command line
    if(argc == 2)
    { 
        EXAMPLE_MESH_SIDE_DIM = atoi(argv[1]);
    }
    
    result = RUN_ALL_TESTS();
    return result;
}


