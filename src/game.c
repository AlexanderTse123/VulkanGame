#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"

int main(int argc,char *argv[])
{
	//mouse position
	int xPos;
	int yPos;

	//variables for weapons
	int weaponid = 2;
	int charge = 0;
	int weaponstate = 0;

    int done = 0;
    int a;
    Uint8 validate = 1;
    const Uint8 * keys;
    Uint32 bufferFrame = 0;
    VkCommandBuffer commandBuffer;
    Model *model;
    Matrix4 modelMat;
    Model *model2;
    Matrix4 modelMat2;
	Model *model3;
	Matrix4 modelMat3;
	Model *model4;
	Matrix4 modelMat4;

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"-disable_validate") == 0)
        {
            validate = 0;
        }
    }
    
    init_logger("gf3d.log");    
    slog("gf3d begin");
    gf3d_vgraphics_init(
        "gf3d",                 //program name
        1200,                   //screen width
        700,                    //screen height
        vector4d(0.51,0.75,1,1),//background color
        0,                      //fullscreen
        validate                //validation
    );
    
    // main game loop
    slog("gf3d main loop begin");
	//create all the weapons
	model = gf3d_model_load("Platform");
	gfc_matrix_identity(modelMat);
	gfc_matrix_make_translation(
		modelMat,
		vector3d(0, 0, 0)
		);
    model2 = gf3d_model_load("Rifle");
    gfc_matrix_identity(modelMat2);
    gfc_matrix_make_translation(
            modelMat2,
            vector3d(0,35,0)
        );
	model3 = gf3d_model_load("Pistol");
	gfc_matrix_identity(modelMat3);
	gfc_matrix_make_translation(
		modelMat3,
		vector3d(0, 0, 0)
		);
	model4 = gf3d_model_load("Unscoped_Sniper");
	gfc_matrix_identity(modelMat4);
	gfc_matrix_make_translation(
		modelMat4,
		vector3d(20, 0, 0)
		);
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame

		//Change weapon
		if (keys[SDL_SCANCODE_1])
		{
			model2 = gf3d_model_load("Unscoped_Sniper");
			gfc_matrix_identity(modelMat2);
			gfc_matrix_make_translation(
				modelMat2,
				vector3d(0, 35, 0)
				);
			weaponid = 1;
		}
		if (keys[SDL_SCANCODE_2])
		{
			model2 = gf3d_model_load("Rifle");
			gfc_matrix_identity(modelMat2);
			gfc_matrix_make_translation(
				modelMat2,
				vector3d(0, 35, 0)
				);
			weaponid = 2;
		}
		if (keys[SDL_SCANCODE_3])
		{
			model2 = gf3d_model_load("Pistol");
			gfc_matrix_identity(modelMat2);
			gfc_matrix_make_translation(
				modelMat2,
				vector3d(0, 35, 0)
				);
			weaponid = 3;
		}

		//left click to shoot
		//Sniper has a charge mechanic
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) & weaponid == 1)
		{
			charge++;
			slog("charging");
		}
		else if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) & charge < 100 & charge > 0 & weaponid == 1)
		{
			charge = 0;
			slog("partial charged shot");
		}
		else if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) & charge >= 100 & charge > 0 & weaponid == 1)
		{
			charge = 0;
			slog("fully charged shot");
		}
		//Rifle has automatic fire
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) & weaponid == 2)
		{
			slog("shoot rifle");
		}
		//Pistol is semiautomatic
		if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) & weaponid == 3 & weaponstate == 0)
		{
			weaponstate = 1;
			slog("shoot pistol");
		}
		else if (!(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) & weaponid == 3 & weaponstate == 1)
		{
			weaponstate = 0;
		}

		/*xPos = GET_X_LPARAM();
		yPos = GET_Y_LPARAM();*/

		if (keys[SDL_SCANCODE_LEFT])
		{
			gf3d_vgraphics_rotate_camera(0.01);
		}
		if (keys[SDL_SCANCODE_RIGHT])
		{
			gf3d_vgraphics_rotate_camera(-0.01);
		}
		if (keys[SDL_SCANCODE_UP])
		{
			gf3d_vgraphics_rotate_camera_vertical(0.0001);
		}
		if (keys[SDL_SCANCODE_DOWN])
		{
			gf3d_vgraphics_rotate_camera_vertical(-0.0001);
		}

		//update game things here
        //gf3d_vgraphics_rotate_camera(0.001);
		//gf3d_vgraphics_rotate_camera_vertical(0.001);
        /*gfc_matrix_rotate(
            modelMat,
            modelMat,
            0.002,
            vector3d(1,0,0));
        gfc_matrix_rotate(
            modelMat2,
            modelMat2,
            0.002,
            vector3d(0,0,1));*/

        // configure render command for graphics command pool
        // for each mesh, get a command and configure it from the pool
        bufferFrame = gf3d_vgraphics_render_begin();
        gf3d_pipeline_reset_frame(gf3d_vgraphics_get_graphics_pipeline(),bufferFrame);
            commandBuffer = gf3d_command_rendering_begin(bufferFrame);

                gf3d_model_draw(model, bufferFrame, commandBuffer, modelMat);
                gf3d_model_draw(model2, bufferFrame, commandBuffer, modelMat2);
				gf3d_model_draw(model3, bufferFrame, commandBuffer, modelMat3);
				gf3d_model_draw(model4, bufferFrame, commandBuffer, modelMat4);
                
            gf3d_command_rendering_end(commandBuffer);
            
        gf3d_vgraphics_render_end(bufferFrame);

		// move all other objects in the world to make it look like the player is moving
		float speed = 1;
		if (keys[SDL_SCANCODE_W])
		{
			//gf3d_camera_move(vector3d(0, speed, 0));
			gfc_matrix_translate(
				modelMat,
				vector3d(0, speed, 0)
				);
			gfc_matrix_translate(
				modelMat3,
				vector3d(0, speed, 0)
				);
			gfc_matrix_translate(
				modelMat4,
				vector3d(0, speed, 0)
				);
		}

		if (keys[SDL_SCANCODE_A])
		{
			//gf3d_camera_move(vector3d(speed, 0, 0)); 
			gfc_matrix_translate(
				modelMat,
				vector3d(-speed, 0, 0)
				);
			gfc_matrix_translate(
				modelMat3,
				vector3d(-speed, 0, 0)
				);
			gfc_matrix_translate(
				modelMat4,
				vector3d(-speed, 0, 0)
				);
		}
		if (keys[SDL_SCANCODE_S])
		{
			//gf3d_camera_move(vector3d(0, -speed, 0));
			gfc_matrix_translate(
				modelMat,
				vector3d(0, -speed, 0)
				);
			gfc_matrix_translate(
				modelMat3,
				vector3d(0, -speed, 0)
				);
			gfc_matrix_translate(
				modelMat4,
				vector3d(0, -speed, 0)
				);
		}
		if (keys[SDL_SCANCODE_D])
		{
			//gf3d_camera_move(vector3d(-speed, 0, 0));
			gfc_matrix_translate(
				modelMat,
				vector3d(speed, 0, 0)
				);
			gfc_matrix_translate(
				modelMat3,
				vector3d(speed, 0, 0)
				);
			gfc_matrix_translate(
				modelMat4,
				vector3d(speed, 0, 0)
				);
		}

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
    }    
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
