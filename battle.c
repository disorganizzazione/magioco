#include "universal.h"
#include <stdio.h>

extern Data_t Local;

void battle(void)
{
  Item_t* Item;
  Enemy_t* Enemy;
  Action_t* Action;

  print_Enemies();
  print_stats();
  switch(Local.phase)
    {
    case'i':
      print_Items();
      if(choice(&Local.item_chosen, Local.Bag.items))
        Local.phase='u';
      break;
    case'u':
      print_Uses(item_sel());
      if(choice(&Local.use_chosen, 3))
	{
	  switch(Local.use_chosen)
	    {
	    case 1: //usa
	      Local.enemy_chosen= 1;
	      Local.phase='1';
	      break;
	    case 2: //lancia
	      Local.enemy_chosen= 1;
	      Local.phase='2';
	      break;
	    case 3: //difenditi
	      if(Local.defending)
	        {
		  Item= addItem(&Local.Bag);
		  Item->Info= Local.Defending;
		}
	      Item= item_sel();
	      Local.defence= Item->Info.defvalue;
	      Local.Defending= Item->Info;
	      Local.defending= 1;//*****
	      Local.phase='a';
	      break;
	    }
	  break;
	}
      else if(Local.state=='q')
	{
	  switch_state();
	  Local.phase='i';
	}
      break;
    case'1':
      Item= item_sel();
      print_sel(Item);
      if(choice(&Local.enemy_chosen, Local.Battle.enemies+1))
	{
	  if(Local.enemy_chosen== Local.Battle.enemies+1)
	    self_damage(Item->Info.damage);
	  Local.phase='a';
	}
      else if(Local.state== 'q')
	{
	  switch_state();
	  Local.enemy_chosen= 0;
	  Local.phase='u';
	}
      break;
    case'2':
      Item= item_sel();
      print_sel(Item);
      break;
    case'a':
      Enemy= Local.Battle.First;
      for(int x=1; x<Local.current_enemy; x++)
	Enemy= Enemy->Next;
      Action= Enemy->First;
      print_Action(Enemy);
      press_a();
      switch(Action->Info.type)
	{
	case'm':
	  if(Local.ranged)
	    break;
	  else if(Local.defence)
	    {
	      resist(Action->Info.value);
	      if(!--Local.defence && Local.defending)
		{
		  Item= addItem(&Local.Bag);
		  Item->Info= Local.Defending;
		  Local.defending= 0;
		}
	    }
	  else
	    self_damage(Action->Info.value);
	  break;
	case'r':
	  if(Local.defending)
	    if(Local.Defending.defvalue < Action->Info.value)
	      self_damage(Action->Info.value);
	    else
	      resist(Action->Info.value);
	  else
	    self_damage(Action->Info.value);
	  break;
	}
      if(Enemy==enemy_sel() && !Local.defending)
	{
	  Item= item_sel();
	  if(!Local.ranged)
	    {
	      if(Action->Info.type=='r')
		break;
	      else
		enemy_damage(Enemy, Item->Info.damage);
	    }
	  else
	    enemy_damage(Enemy, Item->Info.trowvalue);
	}
      
      Enemy->Last->Next= Action;
      Enemy->First= Action->Next;
      Enemy->Last= Action;
      Action->Next= NULL;
      if(Enemy->Next)
	Local.current_enemy++;
      else
	{
	  Local.current_enemy= 1;
	  Local.phase='i';
	}
      break;
    }
}

Enemy_t* enemy_sel()
{
  Enemy_t* Temp= Local.Battle.First;
  for(int x=1; x<Local.enemy_chosen; x++)
    Temp= Temp->Next;
  return Temp;
}

Item_t* item_sel()
{
  Item_t* Temp= Local.Bag.First;
  for(int x=1; x<Local.item_chosen; x++)
    Temp= Temp->Next;
  return Temp;
}

void enemy_damage(Enemy_t* Enemy, int damage) //danno al nemico (compreesa difesa)
{

}

void self_damage(int damage) //danno diretto alla vita del giocatore
{
  if(damage>=0)
    for(int x= 0; x<damage && Local.health; x++)
      Local.health-= 1;
  else
    Local.health-= damage;
}

void resist(int damage) //danno al gicatore difeso
{
  for(int x= 0; x<damage; x++)
    {
      if(Local.Defending.type=='u')
	if(!--Local.Defending.uses)
	  Local.defending= 0;
      //else?
    }
}
