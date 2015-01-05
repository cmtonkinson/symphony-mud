/*
 * $Id: quoteTable.cpp 489 2010-12-23 07:31:11Z cmtonkinson $
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

#include "quoteTable.h"

QuoteTable::QuoteTable( void ) {
  quotes().push_back( Quote( "Arthur C. Clarke", "Any sufficiently advanced technology is indistinguishable from magic." ) );
  quotes().push_back( Quote( "Albert Einstein", "The significant problems we face cannot be solved by the same level of thinking that created them." ) );
  quotes().push_back( Quote( "Bill Bryson", "A computer is a stupid machine with the ability to do incredibly smart things, while computer programmers are smart people with the ability to do incredibly stupid things.  They are, in short, a perfect match." ) );
  quotes().push_back( Quote( "Brian Kernighan", "Debugging is twice as hard as writing the code in the first place.  Therefore, if you write the code as cleverly as possible, you are, by definition, not smart enough to debug it." ) );
  quotes().push_back( Quote( "Charles Petzold", "Owning a computer without programming is like having a kitchen and using only the microwave oven." ) );
  quotes().push_back( Quote( "Cory Doctorow", "Engineers are all basically high-functioning autistics who have no idea how normal people do stuff." ) );
  quotes().push_back( Quote( "David Wheeler", "All problems in computer science can be solved with another level of indirection." ) );
  quotes().push_back( Quote( "Dick Brandon", "Documentation is like sex: when it is good, it is very, very good; and when it is bad, it is better than nothing." ) );
  quotes().push_back( Quote( "Donald Knuth", "We should forget about small efficiencies, say about 97% of the time: premature optimization is the root of all evil." ) );
  quotes().push_back( Quote( "Douglas Adams", "I love deadlines. I like the whooshing sound they make as they fly by." ) );
  quotes().push_back( Quote( "Edsger W. Dijkstra", "How do we convince people that in programming simplicity and clarity - in short: what mathematicians call \"elegance\" - are not a dispensable luxury, but a crucial matter that decides between success and failure?" ) );
  quotes().push_back( Quote( "Edsger W. Dijkstra", "Simplicity is prerequisite for reliability." ) );
  quotes().push_back( Quote( "Edsger W. Dijkstra", "The question of whether computers can think is just like the question of whether submarines can swim." ) );
  quotes().push_back( Quote( "Eric Raymond", "Computer science education cannot make anybody an expert programmer any more than studying brushes and pigment can make somebody an expert painter." ) );
  quotes().push_back( Quote( "Fred Brooks", "Good judgement is the result of experience... Experience is the result of bad judgement." ) );
  quotes().push_back( Quote( "Fred Brooks", "Nine people can't make a baby in a month." ) );
  quotes().push_back( Quote( "Gavin Russell Baker", "C++ : Where friends have access to your private members." ) );
  quotes().push_back( Quote( "Graham Storr", "Program, n.: (1) A magic spell cast upon a computer to enable it to turn your input into error messages. (2) v.t., A pastime similar to banging your head against a wall but with fewer opportunities for reward." ) );
  quotes().push_back( Quote( "The Guild", "Blades: \"Wiggley, you don't have an arsenal.  You're in a pit of noob and you can't get out.\"" ) );
  quotes().push_back( Quote( "The Guild", "Codex: \"You can't be bald and confident?\"\nClara: \"Um... your skull is out all the time.\"" ) );
  quotes().push_back( Quote( "The Guild", "Vork: \"You will be dead to me as a person.  So much so, that were I faced with the decision of saving you or a strange stray dog that I'd never met before from drowning, I would do my best to drown you myself to save the strange dog that I would not even care to keep, but would, afterwards, place in a non-abusive foster home.  Good day, sir!\"" ) );
  quotes().push_back( Quote( "The Guild", "Zaboo: \"There is so much fail in that sentence I can hardly begin.\"" ) );
  quotes().push_back( Quote( "Howard Aiken", "Don't worry about people stealing your ideas.  If it's original, you'll have to ram it down their throats." ) );
  quotes().push_back( Quote( "Isaac Asimov", "The most exciting phrase to hear in science, the one that heralds new discoveries, is not 'Eureka!' but 'That's funny...'" ) );
  quotes().push_back( Quote( "J. Porter Clark", "... sufficiently advanced cluelessness is indistinguishable from malice." ) );
  quotes().push_back( Quote( "Jeff Atwood", "Good programmers never write what they can steal or borrow." ) );
  quotes().push_back( Quote( "Jeremy S. Anderson", "There are two major products that come out of Berkeley: LSD and UNIX. We don't believe this to be a coincidence." ) );
  quotes().push_back( Quote( "The Oregon Trail", "You have died of dysentery." ) );
  quotes().push_back( Quote( "John Ousterhout", "The greatest performance improvement of all is when a system goes from not-working to working." ) );
  quotes().push_back( Quote( "Keith Bostic", "Perl - The only language that looks the same before and after RSA encryption." ) );
  quotes().push_back( Quote( "Martin Fowler", "Any fool can write code that a computer can understand. Good programmers write code that humans can understand." ) );
  quotes().push_back( Quote( "Nathaniel S Borenstein", "It should be noted that no ethically-trained software engineer would ever consent to write a DestroyBaghdad procedure.  Basic professional ethics would instead require him to write a DestroyCity procedure, to which Baghdad could be given as a parameter." ) );
  quotes().push_back( Quote( "P. J. Plauger", "My definition of an expert in any field is a person who knows enough about what's really going on to be scared." ) );
  quotes().push_back( Quote( "Patrick Logan", "It seems to me you can program with discipline or you can program with bondage and discipline. You can't avoid the discipline either way, but bondage appeals to some people." ) );
  quotes().push_back( Quote( "Phil Karlton", "There are only two hard problems in Computer Science: cache invalidation and naming things." ) );
  quotes().push_back( Quote( "Rich Cook", "Programming today is a race between software engineers striving to build bigger and better idiot-proof programs, and the Universe trying to produce bigger and better idiots.  So far, the Universe is winning." ) );
  quotes().push_back( Quote( "Robert Sewell", "If Java had true garbage collection, most programs would delete themselves upon execution." ) );
  quotes().push_back( Quote( "Steve Ballmer", "Linux is a cancer." ) );
  quotes().push_back( Quote( "Thomas A. Edison", "I have not failed. I've just found 10,000 ways that won't work." ) );
  quotes().push_back( Quote( "Unknown", "A programmer started to cuss\nBecause getting to sleep was a fuss\nAs he lay there in bed\nLooping 'round in his head\nwas: while(!asleep()) sheep++;" ) );
  quotes().push_back( Quote( "Unknown", "Before software can be reusable it first has to be usable." ) );
  quotes().push_back( Quote( "Unknown", "Cheap, fast, reliable - pick 2." ) );
  quotes().push_back( Quote( "Unknown", "Confidence, n.: The feeling you have before you understand the situation." ) );
  quotes().push_back( Quote( "Unknown", "Debuggers don't remove bugs. They only show them in slow motion." ) );
  quotes().push_back( Quote( "Unknown", "Java. The elegant simplicity of C++. The blazing speed of Smalltalk." ) );
  quotes().push_back( Quote( "Unknown", "Man is the best computer we can put aboard a spacecraft...and the only one that can be mass produced with unskilled labor." ) );
  quotes().push_back( Quote( "Unknown", "One programmer can do in one day what two programmers can do in two." ) );
  quotes().push_back( Quote( "Unknown", "Profanity is the one language all programmers know best." ) );
  quotes().push_back( Quote( "Unknown", "Saying that Java is good because it works on all platforms is like saying anal sex is good because it works on all genders." ) );
  quotes().push_back( Quote( "Unknown", "To err is human; to make real mess, you need a computer." ) );
  quotes().push_back( Quote( "Unknown", "The best code is no code at all." ) );
  quotes().push_back( Quote( "Unknown", "Theory is when you know something, but it doesn't work.  Practice is when something works, but you don't know why.  Programmers combine theory and practice: Nothing works and they don't know why." ) );
  quotes().push_back( Quote( "Unknown", "There are 10 types of people in this world.  Those who understand binary, and those who don't." ) );
  quotes().push_back( Quote( "Unknown", "There are only two industries that refer to their customers as users." ) );
  quotes().push_back( Quote( "Unknown", "There is not now, nor has there ever been, nor will there ever be, any programming language in which it is the least bit difficult to write bad code." ) );
  quotes().push_back( Quote( "Unknown", "Weeks of coding can save you hours of planning." ) );
  quotes().push_back( Quote( "Unknown", "When all you have is a hammer, everything starts to look like a nail." ) );
  return;
}

QuoteTable::~QuoteTable( void ) {
  quotes().clear();
  return;
}

void QuoteTable::add( const std::string& author, const std::string& quote ) {
  quotes().push_back( Quote( author, quote ) );
  return;
}

Quote QuoteTable::getRandomQuote( void ) const {
  unsigned max = quotes().size();
  unsigned index = rand() % max;
  return quotes()[index];
}
