#include "SecretBox.h"
#include "Sprites.h"
#include "Mario.h"
#include "Mushroom.h"
#include "Sounds.h"

SecretBox::SecretBox(QPoint position,spawnable_t _spawnable,std::string _type) : BouncingBlock()
{
	//todo vedere se si devono inizializza i counter
	// textures
	type = _type;

	if(type== "normal")
	{
		texture_active[0] = Sprites::instance()->get("secret-box-0");
		texture_active[1] = Sprites::instance()->get("secret-box-1");
		texture_active[2] = Sprites::instance()->get("secret-box-2");
		texture_active[3] = Sprites::instance()->get("secret-box-3");
		texture_inactive = Sprites::instance()->get("secret-box-inactive");
	}
	else if(type== "mega")
	{
		texture_active[0] = Sprites::instance()->get("mega-secret-box");
		texture_inactive = Sprites::instance()->get("mega-empty-box");
	}
	
	// make background color (255, 178, 127) transparent
	texture_active[0].setMask(texture_active[0].createMaskFromColor(QColor(255, 178, 127)));
	texture_inactive.setMask(texture_inactive.createMaskFromColor(QColor(255, 178, 127)));

	setPixmap(texture_active[0]);
	setPos(position);

	setZValue(3);
}

void SecretBox::animate()
{
	if (type == "normal")
	{
		if (active)
			setPixmap(texture_active[(animation_counter++ / 20) % 4]);
		else
			setPixmap(texture_inactive);
	}
	else if (type == "mega")
	{
		if (!active)
			setPixmap(texture_inactive);
	}
}
	
	

// @override
void SecretBox::advance()
{
	if(hit_counter >= 0)
	{
		// raising phase
		if (hit_counter < 12)
			hit_counter++;
		
		// falling phase
		else if(hit_counter >= 12 && hit_counter < 24)
		{
			if (hit_counter == 12)
				dir = DOWN;
			hit_counter++;
		}
		else
		{
			hit_counter = -1;
			//spawn();  //spawn of mushroom
		}

		BouncingBlock::advance();
	}
}

void SecretBox::hit(Object *what, Direction fromDir)
{
	// do nothing if not active
	if(!active)
		return;

	Mario* mario = dynamic_cast<Mario*>(what);
	if(mario && fromDir == DOWN)
	{
		// start hit
		hit_counter = 0;

		moving = true;

		// disable box
		active = false;
		
		BouncingBlock::spawn();

		// play box hit sound
		Sounds::instance()->play("bump");
	}
}