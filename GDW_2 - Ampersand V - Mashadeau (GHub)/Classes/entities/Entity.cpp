#include "entities/Entity.h"
#include "Utilities.h"

float * entity::Entity::areaGravity = new float(1.0F); // the default level of gravity for all entities.
const Color4F entity::Entity::CLR_ATK = Color4F::RED; // colour used for attacking collision shapes (i.e. these deal damage)
const Color4F entity::Entity::CLR_DEF = Color4F::BLUE; // colour used for defensive collision shapes (i.e. these take damage)
const Color4F entity::Entity::CLR_NEU = Color4F::GREEN; // colour used when two objects collides (i.e. collision has happened)


entity::Entity::Entity(std::string texture, float globalZOrder) : sprite(Sprite::create())
{
	sprite->setTexture(texture);
	sprite->setAnchorPoint(Vec2(0.5, 0.5)); // anchour point is the middle of the sprite
	sprite->setGlobalZOrder(globalZOrder); // setting the global z order
	sprite->setTag(entity);
	
	// PhysicsBody * collisionBody = PhysicsBody::create(); // adds a physics body so that it can check for collision
	collisionBody = PhysicsBody::create();
	collisionBody->setContactTestBitmask(0xFFFFFFFF);
	collisionBody->setDynamic(false);
	sprite->setPhysicsBody(collisionBody);

}

// releases the sprite 
entity::Entity::~Entity() { sprite->release(); }

// returns the length of time the entity has existed for.
float entity::Entity::getAge() { return age; }

// returns the entity's name.
std::string entity::Entity::getName() { return name; }

// Returns the entity's description.
std::string entity::Entity::getDescription() { return description; }

// returns the entity's sprite
Sprite * entity::Entity::getSprite() const { return sprite; }

// sets the entity's sprite
void entity::Entity::setSprite(Sprite * sprite) { this->sprite = sprite; }

// returns the sprite's texture (image)
Texture2D * entity::Entity::getTexture() const { return sprite->getTexture(); }

// sets the entity's texture via an image path
void entity::Entity::setTexture(std::string image) { sprite->setTexture(image); }

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
magic::magic_t entity::Entity::getMagicType() const { return magicType->getType(); }

// sets the type of magic for the entity. Changing the magic type will result in the defaults for that type being used.
void entity::Entity::setMagicType(magic::magic_t magicType)
{
	magic::MagicTypes * newMagic = new magic::MagicTypes(magicType);
	this->magicType = newMagic;
}

// sets the entity's position
void entity::Entity::setPosition(Vec2 newPos) 
{ 

	/*
	// updates all of the collision shapes so that they're proportional to the player's new position.
	for each(OOP::Primitive * primitive in aabbs)
	{
		// Vec2 pos1 = sprite->getPosition();
		// Vec2 primPos = primitive->getPosition();
		// Vec2 pos3 = primitive->getPosition() + newPos - sprite->getPosition();
		// primitive->setPosition(newPos + primitive->getPosition() - sprite->getPosition());
		primitive->setPosition(primitive->getPosition() + newPos - sprite->getPosition());
	}

	for each(OOP::Primitive * primitive in circles)
		primitive->setPosition(primitive->getPosition() + newPos - sprite->getPosition());
	
	for each(OOP::Primitive * primitive in capsules)
		primitive->setPosition(primitive->getPosition() + newPos - sprite->getPosition());

	*/

	sprite->setPosition(newPos); // this also moves all of the draw nodes attached to the sprite.

	/*
	// changes the positions of all aabbs so that they're consistent with the player's postion.
	for each (OOP::PrimitiveSquare * aabb in aabbs)
		aabb->setPosition(aabb->getPosition() + newPos - sprite->getPosition());
	
	// changes the positions of all circles so that they're consistent with the player's postion.
	for each (OOP::PrimitiveCircle * circ in circles)
		circ->setPosition(circ->getPosition() + newPos - sprite->getPosition());

	// changes the positions of all capsules so that they're consistent with the player's postion.
	for each (OOP::PrimitiveCapsule * caps in capsules)
		caps->setPosition(caps->getPosition() + newPos - sprite->getPosition());
	*/
}

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

/*
//Just use sprite->rotate().

// rotates the entity using the rotation value stored in the class.
Vec2 entity::Entity::rotateEntity(Vec2 acceleration) { return rotateEntity(theta, acceleration); }

// rotates the entity. The rotation factor is assumed to be in radians.
Vec2 entity::Entity::rotateEntity(float theta, Vec2 acceleration)
{
	// degrees to radians - 1 degree = pi/180 radians. 
	// radians to degrees - 1 radian = 180/pi degrees.

	// Rotates Entity
	cocos2d::Mat4 rotation = Mat4::IDENTITY;
	// this will make hte sprite rotate on its centre
	rotation.translate(cocos2d::Vec3(sprite->getCenterRect().size.width * sprite->getAnchorPoint().x, sprite->getCenterRect().size.height * sprite->getAnchorPoint().y, 0.0F));
	rotation.rotateZ(theta); // applys the 'theta' to the rotation matrix.
	// this will move hte sprite back to its original location
	rotation.translate(-cocos2d::Vec3(sprite->getCenterRect().size.width * sprite->getAnchorPoint().x, sprite->getCenterRect().size.height * sprite->getAnchorPoint().y, 0.0F)); // moves it back

	sprite->setAdditionalTransform((cocos2d::Mat4*)(&rotation)); // rotates the image of the entity.

	return umath::rotate(acceleration, theta); // this will return the sprite's acceleration, rotated by the same amount the sprite was.
}
*/

// gets the sprite's opacity as a percentage.
float entity::Entity::getOpacity() { return sprite->getOpacity() / 255; }

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
const std::vector<OOP::Primitive*> const entity::Entity::getCollisionShapes() const { return collisionShapes; }

// checks for collision.
bool entity::Entity::collision(entity::Entity * e2) { return collision(this, e2); }

// checks for collisions
bool entity::Entity::collision(entity::Entity * e1, entity::Entity * e2)
{
	// the positions of the collision shapes are relative to where the sprite is when they become childs of the sprite. Their positions are actually offsets of the sprite's position.
	// basically, the location of the drawNode for the shape stays the same regardless of where the sprite is due to it being relative to the bottom left-corner of the sprite.

	if (e1 == nullptr || e2 == nullptr) // if either one is null, then a 'false' is returned.
		return false;

	// these save the bottom-left-hand-corner of the sprite's texture rect, which is what the positions of the hitboxes are relative to.
	Vec2 e1Bl(e1->getPositionX() - e1->getSprite()->getTextureRect().size.width / 2, e1->getPositionY() - e1->getSprite()->getTextureRect().size.height / 2);
	Vec2 e2Bl(e2->getPositionX() - e2->getSprite()->getTextureRect().size.width / 2, e2->getPositionY() - e2->getSprite()->getTextureRect().size.height / 2);

	OOP::PrimitiveSquare * tempRect1 = nullptr; // a temporary object that stores a rect from e1, in the position it is in overall.
	OOP::PrimitiveSquare * tempRect2 = nullptr; // a temporary object that stores a rect from e2, in the position it is in overall.

	OOP::PrimitiveCircle * tempCirc1 = nullptr; // a temporary object that stores a circle from e1, in the position it is in overall. The 'z' variable holds the radius.
	OOP::PrimitiveCircle * tempCirc2 = nullptr; // a temporary object that stores a circle from e2, in the position it is in overall. The 'z' variable holds the radius.

	// handles all possible collsions. If 'Active' is turned off for either collision shape, then a no collision is checked.
	for each(OOP::Primitive * e1Prim in e1->getCollisionShapes())
	{
		if (!e1Prim->isActive()) // if the primitive is inactive (i.e. the collision has been turned off), it moves onto the next one.
			continue;
		
		// downcasts the first primitive to know which one it is.
		// 
		switch (e1Prim->getId()) 
		{
		case 1: // Square (AABB)
			tempRect1 = new OOP::PrimitiveSquare(e1Bl + ((OOP::PrimitiveSquare *)e1Prim)->getPosition(), ((OOP::PrimitiveSquare *)e1Prim)->m_WIDTH, ((OOP::PrimitiveSquare *)e1Prim)->m_HEIGHT);
			break;

		case 3: // Circle
			tempCirc1 = new OOP::PrimitiveCircle(Vec2(e1Bl.x + ((OOP::PrimitiveCircle *)e1Prim)->getPosition().x, e1Bl.y + ((OOP::PrimitiveCircle *)e1Prim)->getPosition().y), ((OOP::PrimitiveCircle *)e1Prim)->m_RADIUS);
			break;
		case 5: // Capsule
			break;
		}

		if (tempRect1 == nullptr && tempCirc1 == nullptr) // if all of these are nullptrs, then an unusable primitive was found.
			continue;
		
		for each(OOP::Primitive * e2Prim in e2->getCollisionShapes())
		{
			if (!e2Prim->isActive()) // if the primitive is inactive (i.e. the collision has been turned off), it moves onto the next one.
				continue;

			switch (e2Prim->getId()) // downcasts the second primitive to know which one it is.
			{
			case 1: // Square (AABB)
				tempRect2 = new OOP::PrimitiveSquare(e2Bl + ((OOP::PrimitiveSquare *)e2Prim)->getPosition(), ((OOP::PrimitiveSquare *)e2Prim)->m_WIDTH, ((OOP::PrimitiveSquare *)e2Prim)->m_HEIGHT);
				break;

			case 3: // Circle
				tempCirc2 = new OOP::PrimitiveCircle(Vec2(e2Bl.x + ((OOP::PrimitiveCircle *)e2Prim)->getPosition().x, e2Bl.y + ((OOP::PrimitiveCircle *)e2Prim)->getPosition().y), ((OOP::PrimitiveCircle *)e2Prim)->m_RADIUS);
				break;
			case 5: // Capsule
				break;
			}

			if (tempRect2 == nullptr && tempCirc2 == nullptr) // if all of these are nullptrs, then an unusable primitive was found.
				continue;

			// goes in the order of AABB - circle - capsule
			// capsule hasn't been implemented as of yet.
			if (tempRect1 != nullptr && tempRect2 != nullptr) // AABB check (rectangle and rectangle)
			{
				if (umath::aabbCollision(&tempRect1->getRect(), &tempRect2->getRect())) // if true, the collided primitives at their proper location are saved.
				{
					e1->collidedPrimitive = tempRect1;
					e2->collidedPrimitive = tempRect2;
					return true;
				}
			}
			else if (tempRect1 != nullptr && tempCirc2 != nullptr) // rectangle and circle check
			{
				if (umath::aabbCircleCollision(tempRect1->getRect(), tempCirc2->getPosition(), tempCirc2->m_RADIUS)) // if true, the collided primitives at their proper location are saved.
				{
					e1->collidedPrimitive = tempRect1;
					e2->collidedPrimitive = tempCirc2;
					return true;
				}
			}
			else if (tempCirc1 != nullptr && tempRect2 != nullptr) // circle-rectangle check
			{
				if (umath::aabbCircleCollision(tempRect2->getRect(), tempCirc1->getPosition(), tempCirc1->m_RADIUS)) // if true, the collided primitives at their proper location are saved.
				{
					e1->collidedPrimitive = tempCirc1;
					e2->collidedPrimitive = tempRect2;
					return true;
				}
			}
			else if (tempCirc1 != nullptr && tempCirc2 != nullptr) // circle-circle check
			{
				// if true, the collided primitives at their proper location are saved.
				if (umath::circleCollision(tempCirc1->getPosition(), tempCirc1->m_RADIUS, tempCirc2->getPosition(), tempCirc2->m_RADIUS))
				{
					e1->collidedPrimitive = tempCirc1;
					e2->collidedPrimitive = tempCirc2;
					return true;
				}
			}

			// sets all of these to nullptr for the following check.
			tempRect2 = nullptr;
			tempCirc2 = nullptr;
		}

		// sets all of these to nullptr for the following check.
		tempRect1 = nullptr;
		tempCirc1 = nullptr;
	}

	// handles circle collisions for e1
	//for each(OOP::PrimitiveCircle * e1Circle in e1->getCollisionCircles())
	//{
	//	tempCirc1 = Vec3(e1Bl.x + e1Circle->getPosition().x, e1Bl.y + e1Circle->getPosition().y, e1Circle->m_RADIUS); // creates a temporary version of e1 in its overall location.

	//	// circle circle collision
	//	for each(OOP::PrimitiveCircle* e2Circle in e2->getCollisionCircles())
	//	{
	//		tempCirc2 = Vec3(e2Bl.x + e2Circle->getPosition().x, e2Bl.y + e2Circle->getPosition().y, e2Circle->m_RADIUS); // recreates the e2 circle with the proper position.

	//		// if true, the primitives that collided are saved in their resepctive entities.
	//		if (e1Circle->isActive() && e2Circle->isActive() && umath::circleCollision(Vec2(tempCirc1.x, tempCirc1.y), tempCirc1.z, Vec2(tempCirc2.x, tempCirc2.y), tempCirc2.z))
	//		{
	//			e1->collidedPrimitive = e1Circle;
	//			e2->collidedPrimitive = e2Circle;
	//			return true;
	//		}
	//			
	//	}

	//	// circle square collision
	//	for each(OOP::PrimitiveSquare * e2Box in e2->getAABBs())
	//	{
	//		tempRect2 = OOP::PrimitiveSquare(e2Bl + e2Box->getPosition(), e2Box->m_WIDTH, e2Box->m_HEIGHT).getRect(); // creates a new rectangle at the proper position of the collection rect.

	//		// if true, the primitives that collided are saved in their resepctive entities.
	//		if (e1Circle->isActive() && e2Box->isActive() && umath::aabbCircleCollision(&tempRect2, Vec2(tempCirc1.x, tempCirc1.y), tempCirc1.z))
	//		{
	//			e1->collidedPrimitive = e1Circle;
	//			e2->collidedPrimitive = e2Box;
	//			return true;
	//		}
	//			
	//	}

	//	// circle capsule collision
	//	// there is currently no function for circle capsule collision...
	//}

	/*
	for each(OOP::PrimitiveCapsule * e1Capsule in *e1->getCapsules())
	{
		// there's currently no capsule collisions...
	}
	*/

	//if (e1 == nullptr || e2 == nullptr) // if either one is null, then a 'false' is returned.
	//	return false;

	//// handles AABB collisions for e1
	//for each(OOP::PrimitiveSquare * e1Box in e1->getAABBs())
	//{
	//	// square square collision
	//	for each(OOP::PrimitiveSquare * e2Box in e2->getAABBs())
	//	{
	//		if (umath::aabbCollision(&e1Box->getRect(), &e2Box->getRect()))
	//			return true;
	//	}

	//	// square circle collision
	//	for each(OOP::PrimitiveCircle * e2Circle in e2->getCollisionCircles())
	//	{
	//		if (umath::aabbCircleCollision(&e1Box->getRect(), e2Circle->getPosition(), e2Circle->m_RADIUS))
	//			return true;
	//	}

	//	// sqaure capsule collision
	//	// there is currently no function for square capsule collision...
	//}

	//// handles circle collisions for e1
	//for each(OOP::PrimitiveCircle * e1Circle in e1->getCollisionCircles())
	//{
	//	// circle circle collision
	//	for each(OOP::PrimitiveCircle* e2Circle in e2->getCollisionCircles())
	//	{
	//		if (umath::circleCollision(e1Circle->getPosition(), e1Circle->m_RADIUS, e2Circle->getPosition(), e2Circle->m_RADIUS))
	//			return true;
	//	}

	//	// circle square collision
	//	for each(OOP::PrimitiveSquare * e2Box in e2->getAABBs())
	//	{

	//		if (umath::aabbCircleCollision(&e2Box->getRect(), e1Circle->getPosition(), e1Circle->m_RADIUS))
	//			return true;
	//	}

	//	// circle capsule collision
	//	// there is currently no function for circle capsule collision...
	//}

	///*
	//for each(OOP::PrimitiveCapsule * e1Capsule in *e1->getCapsules())
	//{
	//	// there's currently no capsule collisions...
	//}
	//*/
	//
	// return false;

	return false;
}

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

	age += deltaTime; // counts how long an entity has existed for.
}
