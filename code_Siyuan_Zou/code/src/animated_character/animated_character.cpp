#include "animated_character.hpp"

using namespace cgp;

void character_structure::load_and_initialize(filename_loader_structure const &param_loader, affine_rts const &transform)
{
	// std::cout << "loading character" << std::endl;
	animated_model = mesh_skinning_loader(param_loader, transform);
	// std::cout << "skinning loaded" << std::endl;
	for (auto const &entry : animated_model.rigged_mesh)
	{
		std::string name = entry.first;
		// std::cout << "loading mesh : " << name << std::endl;
		drawable[name].initialize_data_on_gpu(entry.second.mesh_deformed);
		// std::cout << "loading texture : " << param_loader.loader_rigged_mesh.at(name).texture << std::endl;
		drawable[name].texture.load_and_initialize_texture_2d_on_gpu(param_loader.loader_rigged_mesh.at(name).texture);
		// if (name == "sword")
		// {
		// 	drawable[name].model.translation = {-0.2,
		// 										0.1,
		// 										0.7};
		// 	drawable[name].model.rotation = rotation_transform::from_axis_angle({0, 1, 0}, -3.14f / 2);
		// 	// drawable[name].model.rotation = rotation_transform::from_axis_angle({1, 0, 0}, 3.14f / 2);
		// }
	}

	// std::cout << "mesh loaded" << std::endl;

	sk_drawable = skeleton_drawable(animated_model.skeleton);
	set_current_animation(animated_model.animation.begin()->first);
	// std::cout << "character loaded" << std::endl;
}

void character_structure::set_current_animation(std::string const &animation_name)
{
	current_animation_name = animation_name;
	timer.event_period = animated_model.animation[current_animation_name].time_max;
}
