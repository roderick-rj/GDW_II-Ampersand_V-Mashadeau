#include "entities/Entity.h"
#include "Utilities.h"

float * entity::Entity::areaGravity = new float(1.0F); // the default level of gravity for all entities.
bool entity::Entity::shapesVisible = false; // change this to 'false' to hide all collision shapes from view.

const Color4F entity::Entity::CLR_ATK = Color4F::RED; // colour used for attacking collision shapes (i.e. these deal damage)
const Color4F entity::Entity::CLR_DEF = Color4F::BLUE; // colour used for defensive collision shapes (i.e. these take damage)
const Color4F entity::Entity::CLR_NEU = Color4F::GREEN; // colour used when two objects collides (i.e. collision has happened)


entity::Entity::Entity(std::string texture, float globalZOrder) : sprite(Sprite::create())
{
	imagePath = texture;
	sprite->setTexture(texture);
	sprite->setAnchorPoint(Vec2(0.5, 0.5)); // anchour point is the middle of the sprite
	sprite->setGlobalZOrder(globalZOrder); // setting the global z order
	sprite->setTag(entity);
}

// releases the sprite 
entity::Entity::~Entity() 
{
	for (OOP::Primitive * p : collisionBodies) // removing all of the primitives from their parents
		p->getPrimitive()->removeFromParent();

	collisionBodies.clear();

	sprite->removeFromParent();
	
	
}

// returns the length of time the entity has existed for.
float entity::Entity::getAge() { return age; }

// returns the entity's name.
const std::string entity::Entity::getName() const { return name; }

// sets the entity's name in the sprite.
void entity::Entity::setName(std::string name)
{
	this->name = name;
	sprite->setName(name);
}

// Returns the entity's description.
const std::string entity::Entity::getDescription() const { return description; }

// sets the description of the entity.
void entity::Entity::setDescription(std::string description) { this->description = description; }

// gets entity's tag
entity::etag entity::Entity::getTag() { return (entity::etag)sprite->getTag(); }

// sets entity's tag
void entity::Entity::setTag(entity::etag tag) { sprite->setTag((int)tag); }

// returns the entity's sprite
Sprite * entity::Entity::getSprite() const { return sprite; }

// sets the entity's sprite
void entity::Entity::setSprite(Sprite * sprite) { this->sprite = sprite; }

// returns the sprite's texture (image)
Texture2D * entity::Entity::getTexture() const { return sprite->getTexture(); }

// sets the entity's texture via an image path
void entity::Entity::setTexture(std::string image) {
	imagePath = image;
	sprite->setTexture(image);
}

// gets the texture file path.
std::string entity::Entity::getTextureFilePath() const { return imagePath; }

// returns hte texture rect used to crop the sprite's image
Rect entity::Entity::getTextureRect() const { return sprite->getTextureRect(); }

// sets the textureRect (cropped area) for the sprite
void entity::Entity::setTextureRect(Rect textureRect) { sprite->setTextureRect(textureRect); }

// sets the textureRect(cropped area) for the sprite
void entity::Entity::setTextureRect(float x, float y, float width, float height, const bool USE_CENTRE)
{
	// USE_CENTRE == TRUE -> if the user wants (x, y) to be the centre point of the tecture rect
	// USE_CENTRE == FALSE -> if the user wants (x, y) to be a corner point
	(USE_CENTRE) ? setTextureRect(Rect(x - width / 2, y - width / 2, width, height)) : setTextureRect(Rect(x, y, width, height));
}

// gets the magic type of the entity
magic::magic_t entity::Entity::getMagic_T() const { return magicType.getType(); }

// gets the magic object of the entity. Use this to get it's level of effectiveness against other types.
magic::MagicType entity::Entity::getMagicType() const { return magicType; }

// sets the type of magic for the entity. Changing the magic type will result in the defaults for that type being used.
void entity::Entity::setMagicType(magic::magic_t magicType) { this->magicType = magic::MagicType(magicType); }

// sets a new magic object for the entity.
void entity::Entity::setMagicType(magic::MagicType newMagic) { magicType = newMagic; }

// sets the entity's position. While all sprite children move with the sprite, their positions don't change (i.e. getPosition() for them would return the same value).
void entity::Entity::setPosition(Vec2 newPos)  { sprite->setPosition(newPos); }

// sets the entity's position
void entity::Entity::setPosition(float x, float y) { setPosition(Vec2(x, y)); }

// Gets the position of the sprite
Vec2 entity::Entity::getPosition() const { return sprite->getPosition(); }

void entity::Entity::setPositionX(float x) { setPosition(x, sprite->getPositionY()); }

// gets the sprite's x position.
float entity::Entity::getPositionX() const { return sprite->getPositionX(); }

void entity::Entity::setPositionY(float y) { setPosition(sprite->getPositionX(), y); }

// gets the sprite's y position.
float entity::Entity::getPositionY() const { return sprite->getPositionY(); }

// gets the sprite's opacity as a percentage.
float entity::Entity::getOpacity() { return sprite->getOpacity() / 255.0F; }

// flips the sprite based on the value of sprFX. If 'aniFX' is 'true', then all of the animations are given this flip factor as well.
void entity::Entity::setFlippedSpriteX(bool sprFX, bool aniFX)
{
	sprite->setFlippedX(sprFX); // flips the sprite

	if (aniFX)
	{
		for (OOP::SpriteSheetAnimation * sprA : animations) // flips all of the animations.
			sprA->setFlippedAnimationX(sprFX);	
	}
}

// flips the sprite on the x-axis. If it was already flipped, it is un-flipped, and vice versa.
// if aniFX is true, then the animations are also flipped in a similar fashion (if flipped, they become un-flipped, and vice-versa).
void entity::Entity::flipSpriteX(bool aniFX)
{
	sprite->setFlippedX(!sprite->isFlippedX()); // makes the flip factor the opposite of what it currently is.

	if (aniFX)
	{
		for (OOP::SpriteSheetAnimation * sprA : animations) // reverses the current animation flip (if flipped, they become unflipped and vice versa).
			sprA->setFlippedAnimationX(!sprA->getFlippedAnimationX());
	}
}

// returns 'true' if the sprite is currently flipped on the x-axis. Do note that the current animation may have flip the sprite.
bool entity::Entity::getFlippedSpriteX() const { return sprite->isFlippedX(); }

// returns 'true' if all of the sprite animations are flipped, false otherwise.
bool entity::Entity::getFlippedSpriteAnimationsX() const
{
	for (OOP::SpriteSheetAnimation * sprAni : animations)
	{
		if (!sprAni->getFlippedAnimationX()) // if the sprite isn't flipped, then a 'false' is returned.
			return false;
	}

	return true;
}

// flips the sprite on the y-axis using 'sprFY'. If 'aniFY' is 'true', then the sprite's animations are flipped on the y-axis as well.
// If 'aniFY' is true, the provided value of 'sprFY' will become the flip factor for ALL ANIMATIONS.
// in other words, if the sprite is flipped (sprFX == true) and aniFX is 'true', then all of the animation frames would share the same flip factor.
void entity::Entity::setFlippedSpriteY(bool sprFY, bool aniFY)
{
	sprite->setFlippedY(sprFY); // flips the sprite

	if (aniFY)
	{
		for (OOP::SpriteSheetAnimation * sprA : animations) // flips all of the animations.
			sprA->setFlippedAnimationY(sprFY);
	}
}

// flips the sprite based on the value of sprFY. If 'aniFY' is 'true', then all of the animations are given this flip factor as well.
void entity::Entity::flipSpriteY(bool aniFX)
{
	sprite->setFlippedY(!sprite->isFlippedY()); // makes the flip factor the opposite of what it currently is.

	if (aniFX)
	{
		for (OOP::SpriteSheetAnimation * sprA : animations) // reverses the current animation flip (if flipped, they become unflipped and vice versa).
			sprA->setFlippedAnimationY(!sprA->getFlippedAnimationY());
	}
}

// returns 'true' if the sprite is currently flipped on the y-axis. Do note that the current animation may have flip the sprite.
bool entity::Entity::getFlippedSpriteY() const { return sprite->isFlippedY(); }

// returns 'true' if all the sprite animations are flipped, false otherwise.
bool entity::Entity::getFlippedSpriteAnimationsY() const
{
	for (OOP::SpriteSheetAnimation * sprAni : animations)
	{
		if (!sprAni->getFlippedAnimationY()) // if the sprite isn't flipped, then a 'false' is returned.
			return false;
	}

	return true;
}

// sets the opacity of the sprite via a percentage.
void entity::Entity::setOpacity(float opacity)
{
	// If an opacity greater than 100% (1.0) is provided, the variable is set to 1.0.
	if (opacity > 1.0F)
	{
		opacity = 1.0F;
	
	// If a negative opacity is provided, it is set to 0.
	}
	else if (opacity < 0.0F)
	{
		opacity = 0.0F;
	}

	sprite->setOpacity(255 * opacity); // sets the sprite's new opacity.
		
}

// returns the mass of the entity
float entity::Entity::getMass() const { return mass; }

// sets the mass of the entity; can't be below or equal to 0.
void entity::Entity::setMass(float mass) { (mass > 0.0F) ? this->mass = mass : this->mass; }

// returns the force on both axes
Vec2 entity::Entity::getForce() const { return force; }

// returns the force on the x-axis
float entity::Entity::getForceX() const { return force.x; }

// returns the force on the y-axis
float entity::Entity::getForceY() const { return force.y; }

// sets the force for both axes
void entity::Entity::setForce(Vec2 force) { this->force = force; }

// sets the force for both axes
void entity::Entity::setForce(float x, float y) { setForce(Vec2(x, y)); }

// adds to the entity's current force
void entity::Entity::addForce(Vec2 force) { this->force += force; }

// adds to the entity's current force
void entity::Entity::addForce(float forceX, float forceY) { force += Vec2(forceX, forceY); }

// states whether acceleration gets rotated or not.
bool entity::Entity::getRotateAcceleration() const { return rotateAccel; }

// sets whether to rotate the acceleration or not.
void entity::Entity::setRotateAcceleration(bool rAccel) { rotateAccel = rAccel; }

// gets the current velocity of the entity.
Vec2 entity::Entity::getVelocity() { return velocity; }

// sets the entity's velocity to 0.
void entity::Entity::zeroVelocity() { setVelocity(Vec2(0.0F, 0.0F)); }

// sets the velocity of the entity.
void entity::Entity::setVelocity(Vec2 velocity) { this->velocity = velocity; }

// zeroes out the entity's's velocity on the x-axis.
void entity::Entity::zeroVelocityX() { setVelocity(Vec2(0.0F, velocity.y)); }

// zeroes out the entity's velocity on the y-axis.
void entity::Entity::zeroVelocityY() { setVelocity(Vec2(velocity.x, 0.0F)); }

// gets the maixmum velocity of the entity
Vec2 entity::Entity::getMaxVelocity() { return maxVelocity; }

// sets the maximum velocity; it cannot be below 0.
void entity::Entity::setMaxVelocity(Vec2 maxVelocity)
{
	// sets the maximum velocity of the entity. Comparisons use the absolute value of the entity, hence why it must be above 0.
	if (maxVelocity > Vec2(0.0F, 0.0F))
		this->maxVelocity = maxVelocity;
}

// gets rotation factor in degrees, which is what it's stored as.
float entity::Entity::getRotationInDegrees() { return sprite->getRotation(); }

// gets rotation factor in radians
void entity::Entity::setRotationInDegrees(float theta) { sprite->setRotation(theta); }

// gets the rotation factor in radians.
float entity::Entity::getRotationInRadians() { return umath::degreesToRadians(sprite->getRotation()); }

// sets the rotation factor in radians
void entity::Entity::setRotationInRadians(float theta) { sprite->setRotation(umath::radiansToDegrees(theta)); }



// returns the deceleration rate of the entity.
Vec2 entity::Entity::getDecelerate() const { return decelerate; }

// sets the deceleration rate of the entity. This should be less than 1 but greater than 0.
void entity::Entity::setDecelerate(Vec2 decelerate)
{
	// if the deceleration rate passed is greater than 0, and less than 1.0 (since this would cause no deceleration).
	if ((decelerate.x > 0.0F && decelerate.x < 1.0F) && (decelerate.y > 0.0F && decelerate.y < 1.0F))
		this->decelerate = decelerate;
}

// returns at what point the entity comes to a complete stop.
Vec2 entity::Entity::getForceStop() const { return forceStop; }

// sets the threshold the entity's velocity must pass (i.e. value the entity's velocity must fall below) to be brought into a complete stop.
void entity::Entity::setForceStop(Vec2 forceStop)
{
	// the provided forceStop must be greater than 0.0F, since a value cannot be divided to become 0.
	// forceStop must also be positive, as it uses the absolute value of the entity's velocity to check if they're below 'forceStop' or not.
	if (forceStop.x > 0.0F && forceStop.y > 0.0F)
		this->forceStop = forceStop;
}

// returns whether the entity has a constant velocity or not.
bool entity::Entity::getConstVelocity() const { return constVelocity; }

// returns the current bool of 'anti gravity', which determines if the entity is affected by gravity or not.
bool entity::Entity::getAntiGravity() const { return antiGravity; }

// sets whether the entity has anti gravity or not.
void entity::Entity::setAntiGravity(float antiGravity) { this->antiGravity = antiGravity; }

// toggles anti gravity on/off.
void entity::Entity::setAntiGravity() { antiGravity = !antiGravity; }




// returns the collision shapes for the entity.
std::vector<OOP::Primitive*> entity::Entity::getCollisionBodies() const { return collisionBodies; }

// sets a vector of collision bodies
void entity::Entity::setCollisionBodies(std::vector<OOP::Primitive*>& colBodies)
{
	for (OOP::Primitive * p : collisionBodies) // removes all of the primitives from the current sprite.
		p->getPrimitive()->removeFromParent();

	collisionBodies.erase(collisionBodies.begin(), collisionBodies.end() + 1); // removes the information in the vector.
	collisionBodies = colBodies; // saves the new values.

	for (OOP::Primitive * p : collisionBodies) // adds all of the new primitives to the sprite.
		sprite->addChild(p->getPrimitive());
}

// gets collision bodies offset by the sprite's position in the scene (or just whatever the sprite is relative to)
std::vector<OOP::Primitive*> entity::Entity::getOffsetCollisionBodies() const { return getOffsetCollisionBodies(getSprite(), getCollisionBodies()); }

// offsets the collision bodies by the sprite because position of attached sprites uses the sprite as the 'canvas', and as such doesn't change position when the sprite does.
std::vector<OOP::Primitive*> entity::Entity::getOffsetCollisionBodies(const cocos2d::Sprite * spr, const std::vector<OOP::Primitive*> & prims)
{
	bool added = false; // used to make sure a primitive was actually added.
	// the new primitives.
	std::vector<OOP::Primitive *> newPrims;
	// the bottom left-hand corner of the sprite
	Vec2 eBl(spr->getPositionX() - spr->getTextureRect().size.width / 2, spr->getPositionY() - spr->getTextureRect().size.height / 2);

	for (OOP::Primitive * ePrim : prims)
	{
		// downcasts the first primitive to know which one it is.
		switch (ePrim->getId())
		{
		case 1: // Square (AABB)
			newPrims.push_back(new OOP::PrimitiveSquare(eBl + ((OOP::PrimitiveSquare *)ePrim)->getPosition(), ((OOP::PrimitiveSquare *)ePrim)->m_WIDTH, ((OOP::PrimitiveSquare *)ePrim)->m_HEIGHT));
			added = true;
			break;

		case 2: // Square (OBB)
			newPrims.push_back(new OOP::PrimitiveOrientedSquare(eBl + ((OOP::PrimitiveOrientedSquare *)ePrim)->getPosition(), ((OOP::PrimitiveOrientedSquare *) ePrim)->m_WIDTH, ((OOP::PrimitiveOrientedSquare *) ePrim)->m_HEIGHT));
			added = true;
			break;

		case 3: // Circle
			newPrims.push_back(new OOP::PrimitiveCircle(Vec2(eBl.x + ((OOP::PrimitiveCircle *)ePrim)->getPosition().x, eBl.y + ((OOP::PrimitiveCircle *)ePrim)->getPosition().y), ((OOP::PrimitiveCircle *)ePrim)->m_RADIUS));
			added = true;
			break;

		case 5: // Capsule
			newPrims.push_back(new OOP::PrimitiveCapsule(eBl + ((OOP::PrimitiveCapsule *) ePrim)->getPosition(), ((OOP::PrimitiveCapsule *) ePrim)->m_RECT_WIDTH, ((OOP::PrimitiveCapsule *) ePrim)->m_RECT_HEIGHT / 2, ((OOP::PrimitiveCapsule *) ePrim)->getRotationInDegrees()));
			added = true;
			break;
		}

		if (added)
			newPrims.at(newPrims.size() - 1)->setActive(ePrim->isActive()); // saves the 'active' parameter to the new entity.
	}

	return newPrims;
}

// adds a collision body to the vector
void entity::Entity::addCollisionBody(OOP::Primitive * prim)
{
	// if it's already in the vector, it won't be added again.
	for each(OOP::Primitive * p in collisionBodies)
	{
		if (p == prim)
			return;
	}

	collisionBodies.push_back(prim);
}

// removes a collision body from the vector
void entity::Entity::removeCollisionBody(OOP::Primitive * prim)
{
	// removes the primitive if it exists in the vector.
	for (int i = 0; i < collisionBodies.size(); i++)
	{
		if (collisionBodies.at(i) == prim) // removes the primitive.
		{
			collisionBodies.erase(collisionBodies.begin() + i);
			return;
		}
	}
}

// sets the collision bodies to be either active or non-active.
void entity::Entity::setActiveCollisionBodies(bool active)
{
	for (OOP::Primitive * p : collisionBodies)
	{
		if (p == nullptr)
			continue;

		p->setActive(active);
	}
}

// disables collision bodies
void entity::Entity::disableCollisionBodies() { setActiveCollisionBodies(false); }

// enables the collision bodies
void entity::Entity::enableCollisionBodies() { setActiveCollisionBodies(true); }


// checks collision between two primitives. If a collision check for this combination doesn't exist, a false is returned.
bool entity::Entity::collision(OOP::Primitive * prim1, OOP::Primitive * prim2) { return OOP::Primitive::collision(prim1, prim2); }

// checks for collisions between a vector of primitives.
bool entity::Entity::collision(std::vector<OOP::Primitive*>& cols1, std::vector<OOP::Primitive*>& cols2)
{
	// creates two entites to give the primitives to.
	entity::Entity * e1 = new entity::Entity();
	entity::Entity * e2 = new entity::Entity();

	// sets the two collision vectors to the respective entities.
	e1->setCollisionBodies(cols1);
	e2->setCollisionBodies(cols2);

	return collision(e1, e2);
}

// checks for collision with the current entity and a passed entity.
bool entity::Entity::collision(entity::Entity * e2) { return collision(this, e2); }

// checks for collisions using collision bodies
bool entity::Entity::collision(entity::Entity * e1, entity::Entity * e2) { return collision(e1, e1->getOffsetCollisionBodies(), e2, e2->getOffsetCollisionBodies()); }

// checks for collisions using two entites and passed collision vectors
bool entity::Entity::collision(entity::Entity * e1, const std::vector<OOP::Primitive *> & e1Bodies, entity::Entity * e2, const std::vector<OOP::Primitive *> & e2Bodies)
{
	// the positions of the collision shapes are relative to where the sprite is when they become childs of the sprite. Their positions are actually offsets of the sprite's position.
		// basically, the location of the drawNode for the shape stays the same regardless of where the sprite is due to it being relative to the bottom left-corner of the sprite.

	if (e1 == nullptr || e2 == nullptr) // if either one is null, then a 'false' is returned.
		return false;

	// These are no longer needed, so you should remove these variables later.
	// these save the bottom-left-hand-corner of the sprite's texture rect, which is what the positions of the hitboxes are relative to.
	Vec2 e1Bl(e1->getPositionX() - e1->getSprite()->getTextureRect().size.width / 2, e1->getPositionY() - e1->getSprite()->getTextureRect().size.height / 2);
	Vec2 e2Bl(e2->getPositionX() - e2->getSprite()->getTextureRect().size.width / 2, e2->getPositionY() - e2->getSprite()->getTextureRect().size.height / 2);


	e1Bl = Vec2(0, 0);
	e2Bl = Vec2(0, 0);

	OOP::PrimitiveSquare * tempRect1 = nullptr; // a temporary object that stores a rect from e1, in the position it is in overall.
	OOP::PrimitiveSquare * tempRect2 = nullptr; // a temporary object that stores a rect from e2, in the position it is in overall.

	OOP::PrimitiveOrientedSquare * tempObb1 = nullptr; // tempory oriented bounding box for e1
	OOP::PrimitiveOrientedSquare * tempObb2 = nullptr; // temporary oriented bounding box for e2

	OOP::PrimitiveCircle * tempCirc1 = nullptr; // a temporary object that stores a circle from e1
	OOP::PrimitiveCircle * tempCirc2 = nullptr; // a temporary object that stores a circle from e2

	OOP::PrimitiveCapsule * tempCap1 = nullptr; // temporary capsule pointer for e1
	OOP::PrimitiveCapsule * tempCap2 = nullptr; // temporary capsule pointer for e2

	for each(OOP::Primitive * e1Prim in e1Bodies)
	{
		if (e1Prim == nullptr || e1Prim->isActive() == false) // if the primitive is inactive (i.e. the collision has been turned off), it moves onto the next one.
			continue;

		for each(OOP::Primitive * e2Prim in e2Bodies)
		{
			if (e2Prim == nullptr || e2Prim->isActive() == false) // if the primitive is inactive (i.e. the collision has been turned off), it moves onto the next one.
				continue;

			
			if (OOP::Primitive::collision(e1Prim, e2Prim)) // if the two primitives have collided.
			{
				e1->collidedPrimitive = e1Prim;
				e2->collidedPrimitive = e2Prim;
				return true;
			}
		}
	}

	return false;

	// OOP::PrimitiveCircle * tempCirc1x;

	//// handles all possible collsions. If 'Active' is turned off for either collision shape, then a no collision is checked.
	//for each(OOP::Primitive * e1Prim in e1Bodies)
	//{
	//	if (e1Prim == nullptr || e1Prim->isActive() == false) // if the primitive is inactive (i.e. the collision has been turned off), it moves onto the next one.
	//		continue;

	//	// downcasts the first primitive to know which one it is.
	//	switch (e1Prim->getId())
	//	{
	//	case 1: // Square (AABB)
	//		tempRect1 = new OOP::PrimitiveSquare(e1Bl + ((OOP::PrimitiveSquare *)e1Prim)->getPosition(), ((OOP::PrimitiveSquare *)e1Prim)->m_WIDTH, ((OOP::PrimitiveSquare *)e1Prim)->m_HEIGHT);
	//		break;

	//	case 2: // Square (OBB)
	//		tempObb1 = new OOP::PrimitiveOrientedSquare(e1Bl + ((OOP::PrimitiveOrientedSquare *)e1Prim)->getPosition(), ((OOP::PrimitiveOrientedSquare *) e1Prim)->m_WIDTH, ((OOP::PrimitiveOrientedSquare *) e1Prim)->m_HEIGHT);
	//		break;

	//	case 3: // Circle
	//		tempCirc1 = new OOP::PrimitiveCircle(Vec2(e1Bl.x + ((OOP::PrimitiveCircle *)e1Prim)->getPosition().x, e1Bl.y + ((OOP::PrimitiveCircle *)e1Prim)->getPosition().y), ((OOP::PrimitiveCircle *)e1Prim)->m_RADIUS);
	//		break;

	//	case 5: // Capsule; this splits the capsule up into two circles and an oriented rectangle.
	//		tempCap1 = new OOP::PrimitiveCapsule(e1Bl + ((OOP::PrimitiveCapsule *) e1Prim)->getPosition(), ((OOP::PrimitiveCapsule *) e1Prim)->m_RECT_WIDTH, ((OOP::PrimitiveCapsule *) e1Prim)->m_RECT_HEIGHT / 2, ((OOP::PrimitiveCapsule *) e1Prim)->getRotationInDegrees());
	//		// tempCirc1 = new OOP::PrimitiveCircle(tempCap1->getCirclePosition1(), tempCap1->m_RADIUS); // gets the circle of
	//		// tempObb1 = tempCap1->getOrientedRect();

	//		// tempCap1 = new OOP::PrimitiveCapsule(Vec2)
	//		break;
	//	}

	//	if (tempRect1 == nullptr && tempCirc1 == nullptr && tempObb1 == nullptr) // if all of these are nullptrs, then an unusable primitive was found.
	//		continue;

	//	for each(OOP::Primitive * e2Prim in e2Bodies)
	//	{
	//		if (e2Prim == nullptr || e2Prim->isActive() == false) // if the primitive is inactive (i.e. the collision has been turned off), it moves onto the next one.
	//			continue;

	//		switch (e2Prim->getId()) // downcasts the second primitive to know which one it is.
	//		{
	//		case 1: // Square (AABB)
	//			tempRect2 = new OOP::PrimitiveSquare(e2Bl + ((OOP::PrimitiveSquare *)e2Prim)->getPosition(), ((OOP::PrimitiveSquare *)e2Prim)->m_WIDTH, ((OOP::PrimitiveSquare *)e2Prim)->m_HEIGHT);
	//			break;

	//		case 2: // Square (OBB)
	//			tempObb2 = new OOP::PrimitiveOrientedSquare(e2Bl + ((OOP::PrimitiveOrientedSquare *)e2Prim)->getPosition(), ((OOP::PrimitiveOrientedSquare *) e2Prim)->m_WIDTH, ((OOP::PrimitiveOrientedSquare *) e2Prim)->m_HEIGHT);
	//			break;

	//		case 3: // Circle
	//			tempCirc2 = new OOP::PrimitiveCircle(Vec2(e2Bl.x + ((OOP::PrimitiveCircle *)e2Prim)->getPosition().x, e2Bl.y + ((OOP::PrimitiveCircle *)e2Prim)->getPosition().y), ((OOP::PrimitiveCircle *)e2Prim)->m_RADIUS);
	//			break;

	//		case 5: // Capsule; this splits the capsule up into two circles and an oriented rectangle.
	//			tempCap2 = new OOP::PrimitiveCapsule(e2Bl + ((OOP::PrimitiveCapsule *) e2Prim)->getPosition(), ((OOP::PrimitiveCapsule *) e2Prim)->m_RECT_WIDTH, ((OOP::PrimitiveCapsule *) e2Prim)->m_RECT_HEIGHT / 2, ((OOP::PrimitiveCapsule *) e2Prim)->getRotationInDegrees());
	//		}

	//		if (tempRect2 == nullptr && tempCirc2 == nullptr && tempObb2 == nullptr) // if all of these are nullptrs, then an unusable primitive was found.
	//			continue;

	//		// goes in the order of AABB - circle - OBB - capsule
	//		// capsule hasn't been implemented as of yet.
	//		if (tempRect1 != nullptr && tempRect2 != nullptr) // AABB check (rectangle and rectangle)
	//		{
	//			if (umath::aabbCollision(&tempRect1->getRect(), &tempRect2->getRect())) // if true, the collided primitives at their proper location are saved.
	//			{
	//				e1->collidedPrimitive = tempRect1;
	//				e2->collidedPrimitive = tempRect2;
	//				return true;
	//			}
	//		}
	//		else if (tempRect1 != nullptr && tempCirc2 != nullptr) // rectangle and circle check
	//		{
	//			if (umath::aabbCircleCollision(tempRect1->getRect(), tempCirc2->getPosition(), tempCirc2->m_RADIUS)) // if true, the collided primitives at their proper location are saved.
	//			{
	//				e1->collidedPrimitive = tempRect1;
	//				e2->collidedPrimitive = tempCirc2;
	//				return true;
	//			}
	//		}
	//		else if (tempRect1 != nullptr && tempObb2 != nullptr) // rectangle and oriented rectangle
	//		{
	//			if (umath::obbCollision(tempRect1->getRect(), 0.0F, tempObb2->getRect(), tempObb2->getRotationInRadians(), false)) // if true, the collided primitives at their proper location are saved.
	//			{
	//				e1->collidedPrimitive = tempRect1;
	//				e2->collidedPrimitive = tempObb2;
	//				return true;
	//			}
	//		}
	//		else if (tempRect1 != nullptr && tempCap2 != nullptr) // rectangle and capsule
	//		{
	//			// collision between the rect and the capsule
	//			if (umath::obbCollision(tempRect1->getRect(), 0.0F, tempCap2->getOrientedRect()->getRect(), tempCap2->getOrientedRect()->getRotationInRadians(), false) ||
	//				umath::aabbCircleCollision(tempRect1->getRect(), tempCap2->getCirclePosition1(), tempCap2->m_RADIUS) ||
	//				umath::aabbCircleCollision(tempRect1->getRect(), tempCap2->getCirclePosition2(), tempCap2->m_RADIUS)
	//				)
	//			{
	//				e1->collidedPrimitive = tempRect1;
	//				e2->collidedPrimitive = tempCap2;
	//				return true;
	//			}
	//		}
	//		
	//		
	//		// tempCircle1
	//		if (tempCirc1 != nullptr && tempRect2 != nullptr) // circle-rectangle check
	//		{
	//			if (umath::aabbCircleCollision(tempRect2->getRect(), tempCirc1->getPosition(), tempCirc1->m_RADIUS)) // if true, the collided primitives at their proper location are saved.
	//			{
	//				e1->collidedPrimitive = tempCirc1;
	//				e2->collidedPrimitive = tempRect2;
	//				return true;
	//			}
	//		}
	//		else if (tempCirc1 != nullptr && tempCirc2 != nullptr) // circle-circle check
	//		{
	//			// if true, the collided primitives at their proper location are saved.
	//			if (umath::circleCollision(tempCirc1->getPosition(), tempCirc1->m_RADIUS, tempCirc2->getPosition(), tempCirc2->m_RADIUS))
	//			{
	//				e1->collidedPrimitive = tempCirc1;
	//				e2->collidedPrimitive = tempCirc2;
	//				return true;
	//			}
	//		}
	//		else if (tempCirc1 != nullptr && tempObb2 != nullptr) // circle-OBB check. Unfortunately, this is just a disguised square-OBB check, since I don't actually understand the algorithm for it.
	//		{
	//			if (umath::obbCollision(
	//				Rect(tempCirc1->getPosition().x - tempCirc1->m_RADIUS, tempCirc1->getPosition().y - tempCirc1->m_RADIUS, tempCirc1->m_RADIUS * 2, tempCirc1->m_RADIUS * 2),
	//				0.0F, tempObb2->getRect(), tempObb2->getRotationInRadians(), false))
	//			{
	//				e1->collidedPrimitive = tempCirc1;
	//				e2->collidedPrimitive = tempObb2;
	//				return true;
	//			}
	//		}
	//		else if (tempCirc1 != nullptr && tempCap2 != nullptr) // circle and capsule
	//		{
	//			if (umath::circleCollision(tempCirc1->getPosition(), tempCirc1->m_RADIUS, tempCap2->getCirclePosition1(), tempCap2->m_RADIUS) ||
	//				umath::circleCollision(tempCirc1->getPosition(), tempCirc1->m_RADIUS, tempCap2->getCirclePosition2(), tempCap2->m_RADIUS) ||
	//				umath::obbCollision(
	//					Rect(tempCirc1->getPosition().x - tempCirc1->m_RADIUS, tempCirc1->getPosition().y - tempCirc1->m_RADIUS, tempCirc1->m_RADIUS * 2, tempCirc1->m_RADIUS * 2), 0.0F,
	//					tempCap2->getOrientedRect()->getRect(), tempCap2->getOrientedRect()->getRotationInRadians(), false)
	//				)
	//			{
	//				e1->collidedPrimitive = tempCirc1;
	//				e2->collidedPrimitive = tempCap2;
	//				return true;
	//			}
	//		}


	//		// sets all of these to nullptr for the following check.
	//		tempRect2 = nullptr;
	//		tempCirc2 = nullptr;
	//		tempObb2 = nullptr;
	//		tempCap2= nullptr;
	//	}

	//	// sets all of these to nullptr for the following check.
	//	tempRect1 = nullptr;
	//	tempCirc1 = nullptr;
	//	tempObb1 = nullptr;
	//	tempCap2 = nullptr;
	//}
}


// gets the animations for the entity.
std::vector<OOP::SpriteSheetAnimation *> entity::Entity::getAnimations() const { return animations; }

// returns the animation at the provided index.
OOP::SpriteSheetAnimation * entity::Entity::getAnimationByIndex(unsigned int index) const
{
	// if the index is outside of the size of 'animations', then a nullptr is immediately returned.
	return (index < animations.size()) ? animations.at(index) : nullptr;
}

// gets an animation by a tag. The first animation with this tag will be returned. If an animation with this tag is not found, a nullptr is returned.
OOP::SpriteSheetAnimation * entity::Entity::getAnimationByTag(int tag)
{
	for (OOP::SSAnimation * ani : animations)
	{
		if (ani->getTag() == tag)
			return ani;
	}

	return nullptr;
}

// returns the index of the passed animation. If the animation does not exist in the vector, a '-1' is returned.
int entity::Entity::getAnimationIndex(const OOP::SpriteSheetAnimation * anime) const
{
	for (int i = 0; i < animations.size(); i++) // checks all indexes
	{
		if (animations.at(i) == anime)
			return i;
	}
	return -1;
}

// returns the current animation. If there is no current animation, a nullptr is returned.
OOP::SpriteSheetAnimation * entity::Entity::getCurrentAnimation() { return currentAnimation; }

// adds an animation to the entity, and returns the index of said animation.
void entity::Entity::addAnimation(OOP::SpriteSheetAnimation * newAnimation)
{
	animations.push_back(newAnimation);
}

// runs an animation based on a provided index. If no animation has this index, then no animation is run.
void entity::Entity::runAnimationByIndex(unsigned int index)
{
	if (index >= animations.size()) // index out of bounds
		return;

	if (currentAnimation != nullptr) // stops the current animation if there is one.
		currentAnimation->stopAnimation();

	currentAnimation = animations.at(index);
	currentAnimation->runAnimation(); // runs the new animation.
}

// runs the animation by the provided tag.
void entity::Entity::runAnimationByTag(int tag)
{
	int index = -1;

	for (int i = 0; i < animations.size(); i++)
	{
		if (animations.at(i)->getTag() == tag) // an animation has been found.
		{
			index = i;
			break;
		}
	}

	if (index >= 0) // if an animation has been found.
		runAnimationByIndex(index);
}

// returns 'true' if an animation is running, false otherwise.
bool entity::Entity::runningAnimation() { return currentAnimation->isRunning(); }

// sets whether the entity has a constant velocity (i.e. the velocity is either '0', or some value) or not.
void entity::Entity::setConstVelocity(bool constVelocity) { this->constVelocity = constVelocity; }

// toggles the 'constVelocity' variable.
void entity::Entity::setConstVelocity() { setConstVelocity(!constVelocity); }




// Update Loop
void entity::Entity::update(float deltaTime)
{
	Vec2 position = getPosition(); // gets the entity's current position
	Vec2 acceleration; // the enemy's current acceleration

	acceleration = force / mass; // gets the force to be applied, divided by the entity's mass.

	if (rotateAccel) // rotates the acceleration.
		acceleration = umath::rotate(acceleration, sprite->getRotation(), true);

	if(antiGravity == false)  // applies gravity to the entity if anti gravity is turned off.
		acceleration.y -= *areaGravity / mass;

	velocity += acceleration * deltaTime; // adds the acceleration to the entity's velocity
	
	// Capping Velocity (x, y)
	// if the entity has gone over its maximum velocity, their speed is set to their maximum velocity (x-axis ver.).
	if (abs(velocity.x * deltaTime) > maxVelocity.x)
		(velocity.x > 0.0F) ? velocity.x = maxVelocity.x / deltaTime : velocity.x = -maxVelocity.x / deltaTime; // checks the direction of the entity to see whether its new velocity should be positive or negative.

	// if the entity has gone over its maximum velocity, their speed is set to their maximum velocity (y-axis ver.).
	if (abs(velocity.y * deltaTime) > maxVelocity.y)
		(velocity.y > 0.0F) ? velocity.y = maxVelocity.y / deltaTime : velocity.y = -maxVelocity.y / deltaTime; // checks the direction of the entity to see whether its new velocity should be positive or negative.


	// Slowing Down (x, y)
	// if the entity does NOT have a constant velocity, currently has a non-zero x-velocity, and has no force being applied to it on the x-axis, it starts to slow down.
	if (velocity.x != 0.0F && force.x == 0.0F)
		velocity.x *= decelerate.x; // multiplied by the deceleration rate

	// if the entity does NOT have a constant velocity, currently has a non-zero y-velocity, and has no force being applied to it on the y-axis, it starts to slow down.
	if (velocity.y != 0.0F && force.y == 0.0F)
		velocity.y *= decelerate.y; // multiplied by the deceleration rate


	// Stopping the Entity (x, y)
	// if the entity does not have a constant velocity, it has no force being applied, has a non-zero x-velocity, and has fallen below 'forceStop' on the x-axis, its x velocity is set to 0.
	if (constVelocity == false && force.x == 0.0F && velocity.x != 0.0F && abs(velocity.x * deltaTime) < forceStop.x)
		velocity.x = 0.0F;

	// if the entity does not have a constant velocity, it has no force being applied, has a non-zero y-velocity, and has fallen below 'forceStop' on the y-axis, its y velocity is set to 0.
	if (constVelocity == false && force.y == 0.0F && velocity.y != 0.0F && abs(velocity.y * deltaTime) < forceStop.y)
		velocity.y = 0.0F;
	
	position += velocity * deltaTime; // adds the velocity to the entity's position.

	setPosition(position); // sets the entity's new position
	setForce(Vec2(0.0F, 0.0F)); // returns the force back to a value of 0.

	if (constVelocity) // if the entity has a constant velocity, the velocity is reset to '0', so values don't carry over.
		velocity = Vec2(0.0F, 0.0F);

	// if there is an animation being run, then the update loop is called.
	if (currentAnimation != nullptr)
		currentAnimation->update(deltaTime);

	age += deltaTime; // counts how long an entity has existed for.
}
