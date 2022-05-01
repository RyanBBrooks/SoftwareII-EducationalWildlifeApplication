/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "model.h"
#include "Box2D/Box2D.h"
#include <QTimer>
#include <iostream>
#include <queue>
#include <QRandomGenerator>

Model::Model()
    : bestiary(new std::vector<Wildlife>())
    , positionWorld(b2Vec2(0.0f, 0.0f))
{
    buildBestiary();

    //create walls, ground, and ceiling
    //reflects 1/10th dimensions of display, can be scaled if desired.
    b2BodyDef wallBodyDef;
    wallBodyDef.type = b2_staticBody;
    wallBodyDef.position.Set(0.0f, 0.0f);
    //add to world
    b2Body* wallBody = positionWorld.CreateBody(&wallBodyDef);
    //create polygons defining the bodies
    b2PolygonShape wallBox;
    wallBox.SetAsBox(15.0f, 1.0f, b2Vec2(0.0f, -2.0f), 0.0f);
    wallBody->CreateFixture(&wallBox, 1.0f);
    wallBox.SetAsBox(15.0f, 1.0f, b2Vec2(0.0f, 30.0f), 0.0f);
    wallBody->CreateFixture(&wallBox, 1.0f);
    wallBox.SetAsBox(1.0f, 15.0f, b2Vec2(-16.0f, 16.0f), 0.0f);
    wallBody->CreateFixture(&wallBox, 1.0f);
    wallBox.SetAsBox(1.0f, 15.0f, b2Vec2(16.0f, 16.0f), 0.0f);
    wallBody->CreateFixture(&wallBox, 1.0f);

    //dynamic body which will bounce around
    b2BodyDef bouncingBodyDef;
    bouncingBodyDef.type = b2_dynamicBody;
    bouncingBodyDef.position.Set(0.0f, 14.0f); //begin at center
    bouncingBodyDef.linearDamping = 0.0f; //do not allow it to slow down
    bouncingBodyDef.angularDamping = 0.0f;
    bouncingBody = positionWorld.CreateBody(&bouncingBodyDef);
    bouncingBody->ApplyLinearImpulse(b2Vec2(-1.8f, 1.1f), bouncingBody->GetWorldCenter(), true);
    b2Vec2 gravity(0,9.8);
    positionWorld.SetGravity(gravity);
    bouncingBody->ApplyForce( bouncingBody->GetMass() * positionWorld.GetGravity(), bouncingBody->GetWorldCenter(), true );

    b2PolygonShape bouncingBodyBox;
    bouncingBodyBox.SetAsBox(1.0f, 1.0f);

    b2FixtureDef bouncingFixtureDef;
    bouncingFixtureDef.shape = &bouncingBodyBox;
    bouncingFixtureDef.density = 1.0f;
    bouncingFixtureDef.friction = 0.0f; // no friction
    bouncingFixtureDef.restitution = 1.0f; //perfectly elastic bounces

    bouncingBody->CreateFixture(&bouncingFixtureDef);
    //start physics loop
    QTimer timer;
    timer.singleShot(5, this, SLOT(updatePositionWorld()));
}

Model::Model(const Model& other)
    : positionWorld(other.positionWorld) {
    bouncingBody=other.bouncingBody;
    xBouncePos=other.xBouncePos;
    yBouncePos=other.yBouncePos;
    bestiary=other.bestiary;
}

Model& Model::operator=(Model other) {
    std::swap(bouncingBody, other.bouncingBody);
    std::swap(positionWorld, other.positionWorld);
    std::swap(yBouncePos, other.yBouncePos);
    std::swap(xBouncePos, other.xBouncePos);
    std::swap(bestiary, other.bestiary);
    return*this;
}

Model::~Model() {
    delete bestiary;
}

void Model::buildBestiary() {
    // alphabetized to look nice

    // begin plants
    Flora baneberry("Bane Berry", // name
                    "A leafy plant with toothy leaves and extremely poisonous red or white berries which " // description
                    "are smooth and waxy in appearance. Both the red and white varieties have singular "
                    "black dots on the berries, distinguishing them from similar fruits. Absolutely do not "
                    "eat the berries.\n\nSource: https://wimastergardener.org/article/baneberry-actaea-spp/",
                    -40); // health modifier
    bestiary->push_back(baneberry);

    Flora jimsonWeed("Jimson Weed", // name
                     "All parts of this plant are toxic and should not be consumed, including the leaves, roots, " // description
                     "seeds, and the white trumpet-like flower that only blooms at night. Once pollinated, the flowers "
                     "form and produce spiky green seedpods which then dry out and turn white. The skin of the pod then "
                     "peels back to reveal dark seeds. When crushed, the leaves of the plant have a pungent odor."
                     "\n\nSource: https://www.bbg.org/news/weed_of_the_month_jimson_weed",
                     -30); // health modifier
    bestiary->push_back(jimsonWeed);

    Flora blueBonnet("Blue Bonnet",
                     "Blue bonnet is a beautiful blue flower that grows up to 10,000 feet above sea level. All parts of it are "
                     "poisonous and should not be consumed, especially the seeds, which grow in pods kind of like beans."
                     "\n\nSource: https://poisoncontrol.utah.edu/plant-guide/lupine.php",
                     -10);
    bestiary->push_back(blueBonnet);

    Flora dandelion("Dandelion",
                    "A short and leafy plant that grows very close to the ground. Has a bright yellow flower with many close-gathered "
                    "petals that turns into a puffy ball of seeds that float on the wind as the plant ages. Often considered to a weed "
                    "due to their profound abundance, dandelions are actually edible, but should be prepared properly before consuming "
                    "when possible.",
                    10);
    bestiary->push_back(dandelion);

    Flora nightshade("Nightshade",
                     "Nightshade is a leafy vine that grows to be quite tall. It's extremely toxic, and can be fatal if consumed. "
                     "It grows berries that ripen into little red balls of poison in late summer and produces purple or blue flowers "
                     "during non-freezing seasons."
                     "\n\nSource: https://poisoncontrol.utah.edu/plant-guide/deadly-nightshade.php",
                     -50);
    bestiary->push_back(nightshade);

    Flora poisonIvy("Poison Ivy",
                    "\"Leaves of three, let it be!\" As the old saying implies, poison ivy should not be touched by bare skin or "
                    "consumed. Red-tinted in the spring, green in the summer, and orange-ish in the fall, poison ivy is a plant with "
                    "waxy leaves that may or may not be accompanied by greenish-white berries and/or citrus-colored flowers."
                    "\n\nSource: https://www.artofmanliness.com/articles/how-to-recognize-poison-ivy-oak-sumac-and-treat-their-rashes/",
                    -5);
    bestiary->push_back(poisonIvy);

    Flora poisonOak("Poison Oak",
                    "The leaves of poison oak may or may not grow in trios, as poison ivy is known to do. Poison oak has waxy leaves that "
                    "range from green to brown throughout the year, and are much rounder and less toothy than those of poison ivy. Still, "
                    "poison oak should not be touched or consumed because it is highly toxic."
                    "\n\nSource: https://www.artofmanliness.com/articles/how-to-recognize-poison-ivy-oak-sumac-and-treat-their-rashes/",
                    -5);
    bestiary->push_back(poisonOak);

    Flora stingingNettle("Stinging Nettle",
                         "Stinging nettle grows in patches to be about 3-4 feet tall and is characterized by being a single, long, very "
                         "strong stem with many large, bulky, toothy leaves. The flowers are close to the stem and have no petals. One "
                         "vital note must be made: stinging nettle has characteristically sharp and visible hairs protruding from the "
                         "length of the stem which, if touched by the skin of an animal, inject an poisonous irritant into flesh. Stinging "
                         "and itching follows for up to 24 hours. If ingested raw, stinging nettle can cause swelling of the throat and "
                         "vomiting. However, if the leaves of the plant are cooked at a high temperature, the plant becomes edible and "
                         "is popular as a tea ingredient in some places."
                         "\n\nSource: https://www.wildflowers-and-weeds.com/Plant_Families/Urticaceae.htmis%20s",
                         -5);
    bestiary->push_back(stingingNettle);
    // end plants

    // begin animals
    Fauna bighornSheep("Bighorn Sheep", // name
                       "The largest wild sheep in North America. Males are quite large and have big, heavy horns which curl " // description
                       "around their heads, and females are about half their size with much smaller horns. Both sexes are "
                       "grey/brown in color and gain a thick underlayer of fur in the winter which they shed in the summer. "
                       "Males battle each other with their large horns for the right to mate with females. They live at high "
                       "altitudes and are well adapted to climbing steep cliff faces to reach food and avoid predators. You "
                       "can watch these animals from afar but they should not be approached!"
                       "\n\nSource: https://www.nps.gov/romo/learn/nature/bighorn_sheep.htm",
                       0,// health modifier
                       QVector<char> {},
                       QVector<char> {'R','P','I','Y','B'},
                       "The Bighorn Sheep is startled by the noise of running. It returns to grazing.", // R result
                       "The Bighorn Sheep nearly steps on you. It narrowly dodges out of the way, confused.", // P result
                       "The Bighorn Sheep tilts its head at you thinking that you look rather silly. It turns and runs away to avoid talking to an awkward human.", // I result
                       "The Bighorn Sheep is startled and runs away at full speed, a look of fear in its eyes.", // Y result
                       "The Bighorn Sheep grazes peacefully."); // B result
    bestiary->push_back(bighornSheep);

    Fauna blackBear("Black Bear", // name
                       "Black bears are medium-sized bears adept at climbing trees. They prefer to avoid humans as much as " // description
                       "possible, but can easily be provoked or drawn towards humans when the proper measures are not taken. "
                       "Always follow all advice given on signs and properly store all odiferous items such as food, toothpaste, "
                       "waste, etc.\n\nWhen traveling through their habitat, travel in groups and be aware of your surroundings. "
                       "If you see a black bear, group up with your party and immediately move out of the area. If the bear "
                       "approaches you, make yourself as large as possible and make a lot of noise. Back away slowly. If you have "
                       "bear spray, make sure it's ready to be used if necessary."
                       "\n\nSource: http://www.bearsmart.com/play/bear-encounters/",
                       -35, // health modifier
                        QVector<char> {'R','P'},
                        QVector<char> {'I','Y','B'},
                       "The Black Bear outruns and catches you, biting your heel.", // R result
                       "The Black Bear bumbles its way over you, as you lie on the ground. It celebrates the easy meal.", // P result
                       "The Black Bear is startled and runs away.", // I result
                       "The Black Bear is startled and runs away to find an easier meal.", // Y result
                       "The Black Bear continues across the path, unconcerned."); // B result
    bestiary->push_back(blackBear);

    Fauna blackWidow("Black Widow", // name
                       "These spiders are one of the only arachnids in Utah that pack a severely venomous bite, even "
                       "potent enough to be deadly in some cases. Though they mostly are seen on the move in the fall when "
                       "they are finding warmer places to set up their webs as weather goes sour, they can also be seen in "
                       "the summer out in the Utah wild. Black widows are nocturnal, and can best be seen at night with a flashlight. "
                       "Its best not to approach this spider, put as much distance as you can between yourself and its venomous "
                       "bite. "
                       "\n\nSource: https://www.standard.net/lifestyle/follow-these-tips-to-ward-off-deadly-black-widow-spiders/article_8471a1ce-2e47-5dce-b22b-c40737598076.html",
                       -50, // health modifier
                        QVector<char> {'P'},
                        QVector<char> {'R','I','Y','B'},
                       "The Black Widow blinks its many eyes at you. Perhaps another time...", // Run away result
                       "The Black Widow crawls on your neck and bites you while you are lying prone.", // Play dead result
                       "The Black Widow blinks its many eyes at you. Perhaps another time...", // Intimidate result
                       "The Black Widow blinks its many eyes at you. Perhaps another time...", // Yell result
                       "The Black Widow blinks its many eyes at you. Perhaps another time..."); // Back away result
    bestiary->push_back(blackWidow);

    Fauna borealToad("Boreal Toad", // name
                       "Found in aquatic environments in higher elevations around Utah. Common places to find "
                       "this toad include: slow-moving streams, beaver dam lakes, and other still water locations. They stand apart "
                       "from other toads because of the white stripe that runs down their back. Boreal toads have no vocal sac and "
                       "therefore do not make sound other than a soft chirping noise. This species is rapidly declining in population "
                       "so if you encounter it in the wild, make sure to respect this chill dude's space."
                       "\n\nSource: https://www.hoglezoo.org/meet_our_animals/animal_finder/boreal_toad/",
                       0, // health modifier
                        QVector<char> {},
                        QVector<char> {'R','P','I','Y','B'},
                       "The Boreal Toad is unbothered.", // R result
                       "The Boreal Toad cuddles a large rock... Wait, that's you!.", // P result
                       "The Boreal Toad is unbothered.", // I result
                       "The Boreal Toad dives into the water and swims away with its powerful legs.", // Y result
                       "The Boreal Toad is unbothered."); // B result
    bestiary->push_back(borealToad);

    Fauna coyote("Coyote", // name
                       "Part of the dog family, you might be able to see the resemblance to your pet at home. However, this beastie "
                       "is not as accustomed to human interactions. Coyotes enjoy a diet of small mammals, insects, reptiles, carrion, "
                       "grasses, and fruit. Its fair to say coyotes are not picky eaters. Its rare to spot this predator in the wild, they're experts in many kinds of "
                       "and will usually spot you far before you spot them. However, if a hungry coyote wanders into your area the best "
                       "way to scare them off is to yell, bang pots, and make as much noise as possible."
                       "\n\nSource: https://www.wildawareutah.org/utah-wildlife-information/coyotes/",
                       -15, // health modifier
                       QVector<char> {'P'},
                       QVector<char> {'R','I','Y','B'},
                       "The Coyote hears you running, and returns to scavenging.", // R result
                       "The Coyote takes a bite out of you, before realizing that you are still alive.", // P result
                       "The Coyote runs away, becoming aware of your deathly glare.", // I result
                       "The Coyote runs away, fearing your screams.", // Y result
                       "The Coyote continues scavenging."); // B result
    bestiary->push_back(coyote);

    Fauna gilaMonster("Gila Monster", // name
                       "This lizard lives in burrows that it digs itself, a natural digger. It prefers the heat, desert, and arid "
                       "temperatures, so if you're in the Utah desert make sure to watch out for it. It holds fat in its tail "
                       "that it can use to sustain itself for months at a time, and as such, it tends to stay in its burrow during "
                       "those times. Beware of this reptiles bite, its one of the few venomous lizards in the world, and while it "
                       "probably won't kill you, it will hurt a lot. Try to give it its respective space."
                       "\n\nSource: hoglezoo.org/meet_our_animals/animal_finder/gila_monster/",
                       -15, // health modifier
                       QVector<char> {'P'},
                       QVector<char> {'R','I','Y','B'},
                       "The Gila Monster rudely ignores you.", // R result
                       "The Gila Monster chomps on one of your finers, poisoning you.", // P result
                       "The Gila Monster rudely ignores you.", // I result
                       "The Gila Monster walks away to its burrow. Sometimes it's just too loud outside.", // Y result
                       "The Gila Monster rudely ignores you."); // B result
    bestiary->push_back(gilaMonster);

    Fauna grizzlyBear("Grizzly Bear", // name
                       "These huge bears are omnivorous, eating a diet of berries, plant roots, berries, small mammals, fish, and hooved animals. "
                       "Grizzly bears build up as much body fat as they can before winter comes where they retire to their den. Though they used to "
                       "populate the entire Rocky Mountain region with regularity, they have landed on the endangered species list and their "
                       "numbers in Utah are dwindling in the single digits. Although it would be very rare to happen upon one of these bears, "
                       "they pose their own dangers. A hungry grizzly bear that is twice the size of a human would be quite the intimidating experience. "
                       "If you camp in grizzly territory, make sure to hang your food high in the trees and away from your campsite, because the smell of "
                       "your food could entice a hungry grizzly into your camp if you store the food on the ground near tents. If you do encounter a grizzly, "
                       "walk away slowly, and hope it doesn't charge you."
                       "\n\nSource: https://www.britannica.com/animal/grizzly-bear",
                       -50, // health modifier
                       QVector<char> {'R','P'},
                       QVector<char> {'I','Y','B'},
                       "The Grizzly Bear chases after you, catching up quickly...", // R result
                       "The Grizzly Bear enjoys its easy meal...", // P result
                       "The Grizzly Bear is confused by your strange demeanor and after some internal debate, bumbles away for an easier meal.", // I result
                       "The Grizzly Bear is confused by your loud yelling and bumbles away for an easier meal.", // Y result
                       "The Grizzly Bear is unaware of your existence."); // B result
    bestiary->push_back(grizzlyBear);

    Fauna kangarooRat("Kangaroo Rat", // name
                       "These wild rodents are found in southern Utah, in the desert with sand perfect for digging burrows "
                       "It usually eats a diet of seeds, and sometimes eats fresh vegetation and insects. This rodent is very skittish "
                       "and shy, and usually only comes out at night. The kangaroo rat has pouches inside its cheek that it uses to store "
                       "food. If you encounter a kangaroo rat in the Utah desert, be considerate."
                       "and leave it in peace."
                       "\n\nSource: https://dwrcdc.nr.utah.gov/rsgis2/Search/Display.asp?FlNm=dipodese",
                       -5, // health modifier
                      QVector<char> {'I'},
                      QVector<char> {'R','P','Y','B'},
                       "The Kangaroo Rat skitters back to its burrow.", // R result
                       "The Kangaroo Rat crawls over a your legs.", // P result
                       "The Kangaroo Rat gets scared and lashes out.", // I result
                       "The Kangaroo Rat skitters back to its burrow at the loud noise.", // Y result
                       "The Kangaroo Rat goes about its day."); // B result
    bestiary->push_back(kangarooRat);

    Fauna mountainLion("Mountain Lion", // name
                       "Solitary animals, these big cats don't stay with their mothers for more than a year and a half before they "
                       "leave to find their own territory. The mountain lion is a skilled hunter, known for being excellent at "
                       "stalking unsuspecting prey through forest and desert until it gets its meal. The prey in question is "
                       "usually deer, so mountain lions can usually be found where the deer are. After catching its meal, a mountain lion "
                       "will bury it under a tree and cover it will with snow, leaves, or soil to save for leftovers. Adult mountain lions "
                       "tend to be from 90 - 200 lbs making them one of the most dangerous animals to encounter in the wild. "
                       "If you encounter a mountain lion, you're in danger because it is likely that the cat is hunting something, you just "
                       "better hope it isn't you. Your best bet is to try to intimidate it by waving your arms and seeming as large as possible."
                       "Yelling at the mountain lion also could help scare it off."
                       "\n\nSource: https://www.wildawareutah.org/utah-wildlife-information/cougars/",
                       -30, // health modifier
                       QVector<char> {'R','P','B'},
                       QVector<char> {'I','Y'},
                       "The Mountain Lion catches you in no time...", // R result
                       "The Mountain Lion celebrates its easy meal ...", // P result
                       "The Mountain Lion is somehow afraid of you.", // I result
                       "The Mountain Lion thinks you are not worth its time.", // Y result
                       "The Mountain Lion closes in on its meal ..."); // B result
    bestiary->push_back(mountainLion);

    Fauna muleDeer("Mule Deer", // name
                       "The smallest type of deer in Utah, but also the most frequently spotted. These deer are the ones that are "
                       "often found wandering through neighborhoods in search of food at all times of the year. The mule deer gets its "
                       "name from the resemblance of its ears to the mule. They follow a migration pattern yearly from higher elevations "
                       "in the summer to lower elevations in the winter. They are most active in the early hours of the morning, and late "
                       "hours of the night. Be careful driving through areas where mule deer are common, they too frequenlty become roadkill. "
                       "Do not feed mule deer, it encourages their reliance on human interaction for sustenance, and therefore brings them "
                       "into dangerous situations. During some seasons, male mule deer can be aggressive so its always better to err on the "
                       "safe side and leave the deer where you see them, and walk away."
                       "\n\nSource: https://www.wildawareutah.org/utah-wildlife-information/mule-deer/",
                       -10, // health modifier
                       QVector<char> {'P'},
                       QVector<char> {'R','I','Y','B'},
                       "The Mule Deer is startled by your footsteps, but it goes back to grazing.", // R result
                       "The Mule Deer accidentally chews on your hair.", // P result
                       "The Mule Deer finds you terrifying and runs away.", // I result
                       "The Mule Deer is annoyed and leaves.", // Y result
                       "The Mule Deer continues about its day."); // B result
    bestiary->push_back(muleDeer);

    Fauna rattlesnake("Rattle Snake", // name
                       "These snakes are types of pit vipers, because of the pit between their nostrils and their eyes. There "
                       "are seven different species of rattlesnakes that can be found in Utah, and all of them are venomous. "
                       "These snakes are also characterized by the rattle that sits at the tip of their tails that they can rattle "
                       "when they feel threatened as a warning. They are proctected under state law, so you should never attempt to "
                       "harm a rattle snake. Because it is illegal and also because going within 5 feet of a rattlesnake is extremely "
                       "dangerous. If you're hiking and hear a rattling noise, keep your dog close and locate the sound of the rattling"
                       "make sure to give the snake a 5 foot radius of space, and remain calm. It is safe to walk away and leave the snake alone."
                       "\n\nSource: https://wildaboututah.org/tag/rattlesnakes/",
                       -30, // health modifier
                       QVector<char> {'I','Y'},
                       QVector<char> {'R','P','B'},
                       "The Rattle Snake slithers merrily on its way.", // R result
                       "The Rattle Snake slithers merrily on its way.", // P result
                       "The Rattle Snake rattles its tail. Unshaken, it lashes out with its venomous fangs ...", // I result
                       "The Rattle Snake rattles its tail your the yelling, and lashes out with its venomous fangs ...", // Y result
                       "The Rattle Snake slithers merrily on its way."); // B result
    bestiary->push_back(rattlesnake);

    Fauna woodScorpion("Wood Scorpion", // name
                       "The wood scorpion is found in the Great Basin region typically. It can glow luminescent in the dark if you encounter "
                       "one in the middle of the night with a blacklight. They glow fluorescent yellow, blue, or green in blacklight. The sting "
                       "of a scorpion is always venomous, though not always that "
                       "harmful to humans. Wood scorpions can be found hiding under stacks of wood and debris. It eats crickets, spiders, and "
                       "other insects. To avoid attracting these dangerous insects to your yard, try to limit its prey in your neighborhood. "
                       "Leave the wood scorpion alone and give it plenty of space."
                       "\n\nSource: https://www.tripsavvy.com/what-to-know-about-scorpions-in-utah-4580503",
                       -20, // health modifier
                       QVector<char> {'I'},
                       QVector<char> {'R','Y','P','B'},
                       "The Wood Scorpion is unbothered.", // R result
                       "The Wood Scorpion is unbothered.", // P result
                       "The Wood Scorpion is scared by the fearsome display and stings you...", // I result
                       "The Wood Scorpion is unbothered.", // Y result
                       "The Wood Scorpion is unbothered."); // B result
    bestiary->push_back(woodScorpion);
    // end animals

    // begin fungi
    Flora chanterelle("Chanterelle", // name
                     "Warning: if you're not sure what kind of mushroom you're looking at, DO NOT EAT IT.\n\n" // description
                     "If you can successfully identify a chanterelle, it is safe to eat once cleaned and cooked. "
                     "Chanterelles can be found in damp areas of Utah forests during mid- to late-summer. They can "
                     "be identified by several characteristics: false gills, sweet smell, orange or yellow cap."
                     "\n\nSource: https://www.mushroom-appreciation.com/chanterelle-mushrooms.html#sthash.aTUARQDx.dpbs",
                     -40); // health modifier
    bestiary->push_back(chanterelle);

    Flora falseMorel("False Morel", // name
                     "Warning: if you're not sure what kind of mushroom you're looking at, DO NOT EAT IT.\n\n" // description
                     "These mushrooms look like the delicious and edible true morel fungus, but are actually "
                     "deadly when consumed by humans. Under no circumstances should you eat a false morel. It "
                     "differs in appearance to its edible lookalike by several identifiers: instead of a pitted cap, "
                     "the false morel bears a wavy and lobed cap. The shape of the false morel is irregular and squat, "
                     "and the cap hangs freely off of the stem."
                     "\n\nSource: https://www.wideopenspaces.com/real-and-false-morel-mushrooms-can-you-tell-the-difference/",
                     -30); // health mod
    bestiary->push_back(falseMorel);

    Flora pennyBun("Penny Bun", // name
                   "Warning: if you're not sure what kind of mushroom you're looking at, DO NOT EAT IT.\n\n" // description
                   "A delicious and safe mushroom.  This mushroom if fortunately not similar to any poisonous mushrooms; however, "
                   "they do look like the Bitter Bolete, which has a very bitter taste.  It has a white top when young, but when mature it"
                   " has a brown, crispy, bun-looking top.  It's stem is thick and white (although, with age, it can turn yellow)."
                   "\n\nSource: https://www.wildfooduk.com/mushroom-guide/cep/",
                   -10); // health mod
    bestiary->push_back(pennyBun);

    // randomly generate a value between -10 and 10 for the puffball's health mod
    uint now = time(0);
    QRandomGenerator healthGenerator(now);
    int randHealth = healthGenerator.generate();
    randHealth = randHealth % 10;
    bool isNeg = randHealth % 2;
    if (isNeg) { randHealth = 0 - randHealth; }

    Flora puffball("Puffball", //name
                   "Warning: if you're not sure what kind of mushroom you're looking at, DO NOT EAT IT.\n\n" // description
                   "Most puffballs are safe, but there are many different types of puffballs.  Puffballs just look like baked potatoes."
                   "  To be absolutely certain your puffball mushroom is safe, you need to cut it open.  The only kind that are guaranteed "
                   "to be safe are the kind that don't have any gills and are pure white on the inside.  Just one characteristic is not "
                   "good enough to identify it as safe to eat."
                   "\n\nSource: https://practicalselfreliance.com/puffball-mushrooms/",
                   randHealth); // health mod
    bestiary->push_back(puffball);

    Flora flyAgaric("Fly Agaric", // name
                    "Warning: if you're not sure what kind of mushroom you're looking at, DO NOT EAT IT.\n\n" // description
                    "The fly agaric is a psychoactive mushroom.  This mushroom can be variations of red or orange.  "
                    "It can also have white spots.  It has gills under the cap of the mushroom that are not connected "
                    "to the stem of the mushroom."
                    "\n\nSource: https://www.woodlandtrust.org.uk/trees-woods-and-wildlife/fungi-and-lichens/fly-agaric/",
                    -35); // health mod
    bestiary->push_back(flyAgaric);

    Flora trueMorel("True Morel", // name
                    "Warning: if you're not sure what kind of mushroom you're looking at, DO NOT EAT IT.\n\n" // description
                    "True morels look kind of like a spongue.  They can be easily confused with the dangerous false morel."
                    "  The true morel has a pitted cap and is hollow in the cap and stem.  The true morel has the cap"
                    " attached to the stem."
                    "\n\nSource: https://www.mushroom-appreciation.com/morel-mushroom.html#sthash.djlEWx1e.dpbs",
                    -20); // health mod
    bestiary->push_back(trueMorel);
    // end fungi
}

void Model::updatePositionWorld() {
    QTimer timer;

    //set up time step
    float32 timeStep = 1.0f / 60.0f; //this shouldn't change, or we would start jumping from position to position
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    // Instruct the world to perform a single step of simulation.
    // It is generally best to keep the time step and iterations fixed.
    positionWorld.Step(timeStep, velocityIterations, positionIterations);

    // Get the position of the bouncing body
    b2Vec2 position = bouncingBody->GetPosition();

    // convert to integer values for convenience
    double nextX = double(position.x);
    double nextY = double(position.y);
    //set appropriate variables
    //bottom left-indexed as per signal information
    xBouncePos = nextX;
    yBouncePos = nextY - 14;
    //continue updating
    emit updateEnemyLocation(xBouncePos, yBouncePos);
    timer.singleShot(5, Qt::PreciseTimer, this, SLOT(updatePositionWorld()) );
}

void Model::getBestiary(bool isPractice){
    //is practice is just passed through because of the inherent structure of button clicks.
    std::vector<Wildlife> bestiaryCopy;
    for(Wildlife w: *bestiary) {
        bestiaryCopy.push_back(w);
    }
    emit retrievedBestiary(bestiaryCopy,isPractice);
}

void Model::givePos() {
    emit updateEnemyLocation(xBouncePos, yBouncePos);
}

