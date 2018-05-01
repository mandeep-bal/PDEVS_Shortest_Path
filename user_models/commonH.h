

#ifndef __COMMON_H
#define __COMMON_H


#define START_PROC		10
#define STOP_PROC		11

#define BRIGHT			2
#define SPEED			15 		//ADDED
#define DARK			1
#define ALL_DARK		0
#define DARK_RIGHT		3		//added
#define DARK_LEFT 		4		//added
#define DELAY_LEFT              19             //added

#define PATTERN			3
#define NO_PATTER		4

#define ON_TRACK		5
#define ON_TRACK2		17		//ADDED
#define OFF_TRACK		6
#define OFF_TRACK2		7
#define OFF_TRACK3		8		//ADDED
#define OFF_TRACK4		18		//ADDED

#define ZERO_TIME		0


namespace COMMON_H {

	enum OutputVal{
		O_STOP,
		O_GO_FWDS,
		O_GO_FWD,
		O_GO_REV,
		O_GO_TURN
	};
}


#endif
