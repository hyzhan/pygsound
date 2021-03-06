
#include "Python.hpp"

#include <memory>
#include <string>
#include <sstream>
#include <fftw3.h>

#include <om/omSound.h>
#include <om/omMath.h>

#include "Context.hpp"
#include "SoundMesh.hpp"
#include "Scene.hpp"
#include "SoundSource.hpp"
#include "Listener.hpp"
#include "MicrophoneArrays.hpp"


namespace py = pybind11;
namespace oms = om::sound;
namespace omm = om::math;


PYBIND11_MODULE(pygsound, ps)
{
	fftw_init_threads();
	fftw_plan_with_nthreads( om::CPU::getCount() );

	py::class_< Context, std::shared_ptr< Context > >( ps, "Context")
            .def(py::init<>())
	        .def_property( "specular_count", &Context::getSpecularCount, &Context::setSpecularCount )
            .def_property( "specular_depth", &Context::getSpecularDepth, &Context::setSpecularDepth )
            .def_property( "diffuse_count", &Context::getDiffuseCount, &Context::setDiffuseCount )
            .def_property( "diffuse_depth", &Context::getDiffuseDepth, &Context::setDiffuseDepth )
            .def_property( "threads_count", &Context::getThreadsCount, &Context::setThreadsCount )
            .def_property( "sample_rate", &Context::getSampleRate, &Context::setSampleRate )
            .def_property( "channel_type", &Context::getChannelLayout, &Context::setChannelLayout );

	py::class_< SoundMesh, std::shared_ptr< SoundMesh > >( ps, "SoundMesh" )
            .def(py::init<>());

	ps.def( "loadobj", &SoundMesh::loadObj, "A function to load mesh and materials",
            py::arg("_path"), py::arg("_basepath"), py::arg("_forceabsorp") = -1.0, py::arg("_forcescatter") = -1.0 );
	ps.def( "createbox", &SoundMesh::createBox, "A function to create a simple shoebox mesh",
			py::arg("_width"), py::arg("_length"), py::arg("_height"), py::arg("_absorp") = 0.5, py::arg("_scatter") = 0.5 );

	py::class_< Scene, std::shared_ptr< Scene > >( ps, "Scene" )
            .def(py::init<>())
//            .def( "saveIR", &Scene::saveIR )
			.def( "setMesh", &Scene::setMesh )
			.def( "computeIR", &Scene::computeIR )
//            .def( "computeBinauralIR", &Scene::computeBinauralIR )
            .def( "computeMultichannelIR", &Scene::computeMultichannelIR );

	py::class_< SoundSource, std::shared_ptr< SoundSource > >( ps, "Source" )
            .def( py::init<std::vector<float>>() )
			.def_property( "pos", &SoundSource::getPosition, &SoundSource::setPosition )
			.def_property( "radius", &SoundSource::getRadius, &SoundSource::setRadius  )
			.def_property( "power", &SoundSource::getPower, &SoundSource::setPower  );

	py::class_< Listener, std::shared_ptr< Listener > >( ps, "Listener" )
            .def( py::init<std::vector<float>>() )
			.def_property( "pos", &Listener::getPosition, &Listener::setPosition )
			.def_property( "radius", &Listener::getRadius, &Listener::setRadius  )
//			.def_property( "channel_layout", &Listener::getChannelLayout, &Listener::setChannelLayout  )
//            .def_property( "channel_layout_type", &Listener::getChannelLayoutType, &Listener::setChannelLayoutType  )
//            .def( "set_array_layout", &Listener::setArrayLayout)
//			.def( "__repr__", [](const Listener &L) {
//                return "<ps.Listener at '" + L.getPosition() + "'>";
//                }
//            )
            ;


	py::enum_< oms::ChannelLayout::Type >( ps, "ChannelLayoutType" )
			.value( "mono", oms::ChannelLayout::MONO )
			.value( "stereo", oms::ChannelLayout::STEREO )
			.value( "binaural", oms::ChannelLayout::BINAURAL )
			.value( "quad", oms::ChannelLayout::QUAD )
			.value( "surround_4", oms::ChannelLayout::SURROUND_4 )
			.value( "surround_5_1", oms::ChannelLayout::SURROUND_5_1 )
			.value( "surround_7_1", oms::ChannelLayout::SURROUND_7_1 )
			.value( "custom", oms::ChannelLayout::CUSTOM )
			.value( "undefined", oms::ChannelLayout::UNDEFINED )
            .export_values();


}
