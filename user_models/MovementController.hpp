
#ifndef MovementController_HPP_
#define MovementController_HPP_

#include <boost/any.hpp>
#include <boost/simulation/pdevs/atomic.hpp>
#include "commonH.h"
#include "SWO.h"

namespace boost {
namespace simulation {
namespace pdevs {
namespace basic_models {
/**
 * @brief MovementController PDEVS Model
 *
 * MovementController PDEVS Model():
 * - X = {(mctrl_lsdata_in, {ON_TRACK, TURN_LEFT, TURN_RIGHT}); {(mctrl_proxydata_in, { NO_OBSTACLE,  STOP })}
 * - Y = {(mctrl_motor1_out, {goFwd, goBkwd stop}); (mctrl_motor2_out, {goFwd, goBkwd, stop})}
 * - S = {“PREP_MOVE_FWD,” “MOVE_FWD,” “PREP_TURN_LEFT,” TURN_LEFT “PREP_TURN_RIGHT, TURN_RIGHT” “PREP_STOP,” “STOP”, "IDLE"}
 * - internal -> See DEVS Spec Doc
 * - external -> See DEVS Spec Doc
 * - out      -> See DEVS Spec Doc
 * - advance  -> See DEVS Spec Doc
*/
template<class TIME, class MSG>
class MovementController : public atomic<TIME, MSG>
{
    TIME _next;
    std::vector<MSG> _outvalue;
private:
    enum State {IDLE, PREP_MOVE_FWD,PREP_MOVE_FWDS, MOVE_FWD,MOVE_FWDS, MX_STOP, WAIT_DATA, PREP_TURN, TURN_ALPHA, PREP_STOP, PREP_TURN2,PREP_TURN3,PREP_TURN4, MX_STOP2,MX_STOP3,MX_STOP4};
    enum Port {mctrl_sctrl_in, mctrl_sctrl_out, mctrl_moveL_out, mctrl_moveR_out};
    std::string portName[4];
	const TIME infinity = boost::simulation::model<TIME>::infinity;
    MSG _outputMessage1,_outputMessage2;
    State _state;
    // Variable
    int sctrl_input = 0;
	// Prep time
	TIME movePrepTime = TIME(00,00,00,050);
	TIME turnTime = TIME(00,00,01,000);
	TIME uturnTime = TIME(00,00,20,000);


public:
    /**
     * @brief MovementController constructor.
     *
     */
    explicit MovementController(const std::string &n = "mctrl") noexcept : _next(infinity), _outvalue(std::vector<MSG>{}),_state(State::IDLE), _outputMessage1(), _outputMessage2() {
    	portName[0] = "SCTRL_OUT"; portName[1] = "MCTRL_OUT"; portName[2] = "motor1"; portName[3] = "motor2";
    	printf("MCTRL CREATED \n");
    }

    /**
     * @brief internal function.
     */
    void internal() noexcept {
    	switch (_state){
    	case PREP_MOVE_FWDS:
    	    	_state = MOVE_FWDS;
    	    	_next = infinity;
    	    	break;

    		case PREP_MOVE_FWD:
    			_state = MOVE_FWD;
    			_next = infinity;
    			break;

    		case MX_STOP:
    		    _state = PREP_TURN;
    		    _next = movePrepTime;
    		    break;

    		//added
    		case MX_STOP2:
				_state = PREP_TURN2;
				_next = movePrepTime;
				break;

				//added
				    		case MX_STOP3:
								_state = PREP_TURN3;
								_next = movePrepTime;
								break;
								//added
				    		case MX_STOP4:
				    						_state = PREP_TURN4;
				    						_next = movePrepTime;
				    						break;

    		case PREP_TURN:
    			_state = TURN_ALPHA;
    			_next = turnTime;
    			break;

    		case PREP_TURN2:
    			_state = TURN_ALPHA;
    		    _next = turnTime;
    		    break;

    		case PREP_TURN3:
    		    			_state = TURN_ALPHA;
    		    		    _next = turnTime;
    		    		    break;

    		case PREP_TURN4:
    		    			_state = TURN_ALPHA;
    		    		    _next = turnTime;
    		    		    break;
    		case TURN_ALPHA:
    			_state = WAIT_DATA;
    			_next = infinity;
    			break;

    		case PREP_STOP:
    			_state = IDLE;
    			_next = infinity;
    			break;

    		default:
    			break;
    	}
    }
    /**
     * @brief advance function.
     * @return Time until next internal event.
     */
    TIME advance() const noexcept {
    	return _next;
    }
    /**
     * @brief out function.
     * @return MSG defined below.
     */
    std::vector<MSG> out() const noexcept {
    	if (_state==PREP_MOVE_FWDS)
    	    	{
    				_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_FWDS);
    				_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_FWDS);
    				return std::vector<MSG>{_outputMessage1, _outputMessage2};

    	    	}
    	else if (_state==PREP_MOVE_FWD)
    	{
			_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_FWD);
			_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_FWD);
			return std::vector<MSG>{_outputMessage1, _outputMessage2};

    	}

    	else if(_state== MX_STOP || _state== MX_STOP2 || _state== MX_STOP3 || _state== MX_STOP4 ||_state==TURN_ALPHA || _state== PREP_STOP)
    	{
			_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_STOP);
			_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_STOP);
			return std::vector<MSG>{_outputMessage1, _outputMessage2};
    	}

    	else if (_state== PREP_TURN)
    	{
			_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_FWD);
			_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_REV);
//    		_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_REV);
//    		_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_REV);
			return std::vector<MSG>{_outputMessage1, _outputMessage2};
    	}

    	//added
    	else if (_state== PREP_TURN2)
		{
			_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_REV);
			_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_FWD);
//    		_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_REV);
//    		_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_REV);
			return std::vector<MSG>{_outputMessage1, _outputMessage2};
		}

    	//added
    	    	else if (_state== PREP_TURN3)
    			{
    				_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_TURN);
    				_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_TURN);
    	//    		_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_REV);
    	//    		_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_REV);
    				return std::vector<MSG>{_outputMessage1, _outputMessage2};
    			}
    	    	else if (_state== PREP_TURN4)
    	    	    			{
    	    	    				_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_REV);
    	    	    				_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_FWD);
    	    	    	//    		_outputMessage1 = MSG(portName[mctrl_moveR_out], COMMON_H::O_GO_REV);
    	    	    	//    		_outputMessage2 = MSG(portName[mctrl_moveL_out], COMMON_H::O_GO_REV);
    	    	    				return std::vector<MSG>{_outputMessage1, _outputMessage2};
    	    	    			}
    	return std::vector<MSG>{};
    }
    /**
     * @brief external function will set the new state depending on the value of the input.
     * @param msg external input message.
     * @param t time the external input is received.
     */
    void external(const std::vector<MSG>& mb, const TIME& t) noexcept {

    	MSG msg = mb.back();
    	if (msg.port == portName[mctrl_sctrl_in]) {

    		sctrl_input = static_cast<int>(msg.val);

    		if (sctrl_input == STOP_PROC) {
    			_state = PREP_STOP;
    			_next = Time::Zero;
    		}

    		else if (sctrl_input == ON_TRACK2) {
    		 if (_state == WAIT_DATA || _state == IDLE || _state == TURN_ALPHA || _state == MOVE_FWD) {
    				_state = PREP_MOVE_FWDS;
    				_next = movePrepTime;
    			}
    		}
    		else if (sctrl_input == ON_TRACK) {

    			if (_state == WAIT_DATA || _state == IDLE || _state == TURN_ALPHA || _state == MOVE_FWDS) {
    		    				_state = PREP_MOVE_FWD;
    		    				_next = movePrepTime;
    		    			}
    		    		}

    		else if (sctrl_input == OFF_TRACK2) {

    		    			if (_state == MOVE_FWD || _state == MOVE_FWDS) {
    		    				_state = MX_STOP;
    		    				_next = Time::Zero;
    		    			}
    		    			else if (_state == WAIT_DATA || _state == IDLE) {
    		    				_state = PREP_TURN;
    		    				_next = movePrepTime;
    		    			}
    		    		}

    		    		//added
    		    		else if (sctrl_input == OFF_TRACK3) {

    						if (_state == MOVE_FWD || _state == MOVE_FWDS) {
    							_state = MX_STOP2;
    							_next = Time::Zero;
    						}
    						else if (_state == WAIT_DATA || _state == IDLE) {
    							_state = PREP_TURN2;
    							_next = movePrepTime;
    						}
    		    		}
    		    		else if (sctrl_input == OFF_TRACK) {

    		    			if (_state == MOVE_FWD || _state == MOVE_FWDS) {
    		    				_state = MX_STOP3;
    		    		     	_next = Time::Zero;
    		    		    	}
    		    			else if (_state == WAIT_DATA || _state == IDLE) {
    		    		    	_state = PREP_TURN3;
    		    		    	_next = movePrepTime;
    		    		    		    		    			}
    		    		    		    		    		}
    		    		else if (sctrl_input == OFF_TRACK4) {

    		    		    		    			if (_state == MOVE_FWD || _state == MOVE_FWDS) {
    		    		    		    				_state = MX_STOP4;
    		    		    		    		     	_next = Time::Zero;
    		    		    		    		    	}
    		    		    		    			else if (_state == WAIT_DATA || _state == IDLE) {
    		    		    		    		    	_state = PREP_TURN4;
    		    		    		    		    	_next = movePrepTime;
    		    		    		    		    		    		    			}
    		    		    		    		    		    		    		}
    	}
    }

    /**
     * @brief confluence function.
     * Execute the internal.
     *
     * @param msg
     * @param t time the external input is confluent with an internal transition.
     */
    void confluence(const std::vector<MSG>& mb, const TIME& t)  noexcept  {
    	internal();
    }

    void print()  noexcept {
    	SWO_PrintString("mctrl");
    }

};
}
}
}
}


#endif /* MovementController_HPP_ */
