/*
 * $Id: mob.cpp 454 2010-08-02 18:38:16Z cmtonkinson@gmail.com $
 *
 * This file is part of the Symphony project <http://code.google.com/p/symphonymud/>
 * Copyright 2005-2010 Chris Tonkinson <cmtonkinson@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "area.h"
#include "command.h"
#include "commandTable.h"
#include "io-handler.h"
#include "mob.h"
#include "mysql.h"
#include "world.h"

Mob::Mob( void ) {
  level( 1 );
  exp( 0 );
  tnl( 1000 );
  return;
}

Mob::Mob( const Mob& ref ): Creature( ref ) {
  vnum( ref.vnum() );
  return;
}

Mob::Mob( ROW row ) {
  ID( row["mobID"] );
  vnum( row["vnum"] );
  gender().set( (unsigned)row["gender"] );
  identifiers().shortname( row["shortname"] );
  identifiers().longname( row["longname"] );
  identifiers().unserializeKeywords( row["keywords"] );
  identifiers().description( row["description"] );
  level( row["level"] );
  exp( row["exp"] );
  tnl( row["tnl"] );
  health( row["health"] );
  maxHealth( row["maxHealth"] );
  mana( row["mana"] );
  maxMana( row["maxMana"] );
  movement( row["movement"] );
  maxMovement( row["maxMovement"] );
  strength( row["strength"] );
  maxStrength( row["maxStrength"] );
  dexterity( row["dexterity"] );
  maxDexterity( row["maxDexterity"] );
  constitution( row["constitution"] );
  maxConstitution( row["maxConstitution"] );
  intelligence( row["intelligence"] );
  maxIntelligence( row["maxIntelligence"] );
  focus( row["focus"] );
  maxFocus( row["maxFocus"] );
  creativity( row["creativity"] );
  maxCreativity( row["maxCreativity"] );
  charisma( row["charisma"] );
  maxCharisma( row["maxCharisma"] );
  luck( row["luck"] );
  maxLuck( row["maxLuck"] );
  armor( row["armor"] );
  bash( row["bash"] );
  slash( row["slash"] );
  pierce( row["pierce"] );
  exotic( row["exotic"] );
  return;
}

Mob::Mob( Area* area, const unsigned long& vnum ): Creature() {

  try {
    char query[MAX_BUFFER];

    sprintf( query, "INSERT IGNORE INTO mobs ( areaID, vnum ) VALUES ( %lu, %lu );", area->ID(), vnum );
    World::Instance().getMysql()->insert( query );
    ID( World::Instance().getMysql()->getInsertID() );

    area->mobs().insert( std::make_pair( vnum, this ) );
    this->vnum( vnum );
    level( 1 );

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to create mob for area %lu: %s\n", area->ID(), me.getMessage().c_str() );
    return;
  }

  return;
}

Mob::~Mob( void ) {
  return;
}

/******************************************************* Overloads of pure virtual methods ********************************************************/
bool Mob::save( void ) {
  try {
    Mysql* mysql = World::Instance().getMysql();
    char query[MAX_BUFFER];

    sprintf( query,
      "UPDATE mobs SET            \
        `gender` = '%s',          \
        `keywords` = '%s',        \
        `shortname` = '%s',       \
        `longname` = '%s',        \
        `description` = '%s',     \
        `level` = %hd,            \
        `exp` = %lu,              \
        `tnl` = %lu,              \
        `health` = %hu,           \
        `maxHealth` = %hu,        \
        `mana` = %hu,             \
        `maxMana` = %hu,          \
        `movement` = %hu,         \
        `maxMovement` = %hu,      \
        `strength` = %hu,         \
        `maxStrength` = %hu,      \
        `dexterity` = %hu,        \
        `maxDexterity` = %hu,     \
        `constitution` = %hu,     \
        `maxConstitution` = %hu,  \
        `intelligence` = %hu,     \
        `maxIntelligence` = %hu,  \
        `focus` = %hu,            \
        `maxFocus` = %hu,         \
        `creativity` = %hu,       \
        `maxCreativity` = %hu,    \
        `charisma` = %hu,         \
        `maxCharisma` = %hu,      \
        `luck` = %hu,             \
        `maxLuck` = %hu,          \
        `armor` = %hd,            \
        `bash` = %hd,             \
        `slash` = %hd,            \
        `pierce` = %hd,           \
        `exotic` = %hd,           \
       WHERE mobID = %lu          \
       LIMIT 1;",
      Mysql::addslashes(gender().string()).c_str(),
      Mysql::addslashes(identifiers().serializeKeywords()).c_str(),
      Mysql::addslashes(identifiers().shortname()).c_str(),
      Mysql::addslashes(identifiers().longname()).c_str(),
      Mysql::addslashes(identifiers().description()).c_str(),
      level(),
      exp(),
      tnl(),
      health(),
      maxHealth(),
      mana(),
      maxMana(),
      movement(),
      maxMovement(),
      strength(),
      maxStrength(),
      dexterity(),
      maxDexterity(),
      constitution(),
      maxConstitution(),
      intelligence(),
      maxIntelligence(),
      focus(),
      maxFocus(),
      creativity(),
      maxCreativity(),
      charisma(),
      maxCharisma(),
      luck(),
      maxLuck(),
      armor(),
      bash(),
      slash(),
      pierce(),
      exotic(),
      ID()
    );
    mysql->update( query );

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to save mob %lu: %s\n", ID(), me.getMessage().c_str() );
    return false;
  }

  return true;
}

bool Mob::destroy( void ) {
  unsigned long tempID = ID();
  try {
    char query[MAX_BUFFER];

    sprintf( query,
      " DELETE                \
        FROM mobs             \
        WHERE mobID = %lu     \
        LIMIT 1;",
      ID()
    );
    World::Instance().getMysql()->remove( query );
    delete this;

  } catch ( MysqlException me ) {
    fprintf( stderr, "Failed to delete mob %lu: %s\n", tempID, me.getMessage().c_str() );
    return false;
  }
  return true;
}

/******************************************************* Overloads of virtual methods ********************************************************/



/******************************************************* Static methods ********************************************************/
Mob* Mob::create( Area* area, const unsigned long& vnum ) {
  return new Mob( area, vnum );
}

Mob* Mob::create( ROW row ) {
  return new Mob( row );
}

Mob* Mob::create( Mob* mob, Room* room ) {
  Mob* m = new Mob( *mob );
  m->room( room );
  return m;
}

std::string Mob::getInformation( Mob* mob ) {
  std::string output;
  char buffer[MAX_BUFFER];

  output.append( "  --== {Ybasic mob data{x ==--\n" );
  // Basic mob information...
  sprintf( buffer, "vnum......... {y%lu{x\n\
level........ {y%u{x\n\
gender....... {y%s{x\n\
keywords..... {y%s{x\n\
shortname.... %s\n\
longname..... %s\n\n\
  --== {Y description{x ==--\n%s\n\
",  mob->vnum(), mob->level(), mob->gender().string().c_str(), mob->identifiers().getKeywordList().c_str(),
    mob->identifiers().shortname().c_str(), mob->identifiers().longname().c_str(),
    mob->identifiers().description().c_str()
  );
  output.append( buffer );

  return output;
}
