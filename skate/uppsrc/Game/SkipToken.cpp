#include "Game.h"

// Definition of the SkipToken function.
// This can be #included in the game code, and in PC utilities.
// It is in this file so that only this file needs to be updated when SkipToken
// needs to be modified.

// If included in PC code, then uint8 and Dbg_MsgAssert will need to be defined.


// Returns a pointer to the next token after p_token.
// It won't necessarily skip over the complete format of the data that is expected
// to follow, it will just skip over enough that it returns a pointer to a token again.
// For example, ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE must be followed by a ESCRIPTTOKEN_PAIR,
// but SkipToken will not check for that and skip over the pair token too, it will just skip over the
// RANDOM_RANGE token and return a pointer to the ESCRIPTTOKEN_PAIR
// So if SkipToken is passed a pointer to a token, it is guaranteed to return a pointer to token, namely
// the nearest next one.
namespace Script {

uint8 *SkipToken(uint8 *p_token)
{
    switch (*p_token)
    {
        case Script::ESCRIPTTOKEN_ENDOFFILE:
            Dbg_MsgAssert(0,("Tried to skip past EndOfFile token"));
            break;
	    case Script::ESCRIPTTOKEN_ENDOFLINE:
        case Script::ESCRIPTTOKEN_EQUALS:
        case Script::ESCRIPTTOKEN_DOT:
        case Script::ESCRIPTTOKEN_COMMA:
        case Script::ESCRIPTTOKEN_MINUS:
        case Script::ESCRIPTTOKEN_ADD:
        case Script::ESCRIPTTOKEN_DIVIDE:
        case Script::ESCRIPTTOKEN_MULTIPLY:
        case Script::ESCRIPTTOKEN_OPENPARENTH:
        case Script::ESCRIPTTOKEN_CLOSEPARENTH:
        case Script::ESCRIPTTOKEN_SAMEAS:
        case Script::ESCRIPTTOKEN_LESSTHAN:
        case Script::ESCRIPTTOKEN_LESSTHANEQUAL:
        case Script::ESCRIPTTOKEN_GREATERTHAN:
        case Script::ESCRIPTTOKEN_GREATERTHANEQUAL:
        case Script::ESCRIPTTOKEN_STARTSTRUCT:
        case Script::ESCRIPTTOKEN_STARTARRAY:
        case Script::ESCRIPTTOKEN_ENDSTRUCT:
        case Script::ESCRIPTTOKEN_ENDARRAY:
        case Script::ESCRIPTTOKEN_KEYWORD_BEGIN:
        case Script::ESCRIPTTOKEN_KEYWORD_REPEAT:
        case Script::ESCRIPTTOKEN_KEYWORD_BREAK:
        case Script::ESCRIPTTOKEN_KEYWORD_SCRIPT:
        case Script::ESCRIPTTOKEN_KEYWORD_ENDSCRIPT:
        case Script::ESCRIPTTOKEN_KEYWORD_IF:
        case Script::ESCRIPTTOKEN_KEYWORD_ELSE:
        case Script::ESCRIPTTOKEN_KEYWORD_ELSEIF:
        case Script::ESCRIPTTOKEN_KEYWORD_ENDIF:
        case Script::ESCRIPTTOKEN_KEYWORD_RETURN:
		case Script::ESCRIPTTOKEN_KEYWORD_ALLARGS:
		case Script::ESCRIPTTOKEN_ARG:
		case Script::ESCRIPTTOKEN_OR:
		case Script::ESCRIPTTOKEN_AND:
		case Script::ESCRIPTTOKEN_XOR:
		case Script::ESCRIPTTOKEN_SHIFT_LEFT:
		case Script::ESCRIPTTOKEN_SHIFT_RIGHT:
		case Script::ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE:
		case Script::ESCRIPTTOKEN_KEYWORD_RANDOM_RANGE2:
		case Script::ESCRIPTTOKEN_KEYWORD_NOT:
		case Script::ESCRIPTTOKEN_KEYWORD_AND:
		case Script::ESCRIPTTOKEN_KEYWORD_OR:
		case Script::ESCRIPTTOKEN_KEYWORD_SWITCH:
		case Script::ESCRIPTTOKEN_KEYWORD_ENDSWITCH:
		case Script::ESCRIPTTOKEN_KEYWORD_CASE:
		case Script::ESCRIPTTOKEN_KEYWORD_DEFAULT:
		case Script::ESCRIPTTOKEN_COLON:
            ++p_token;
			break;

        case Script::ESCRIPTTOKEN_NAME:
        case Script::ESCRIPTTOKEN_INTEGER:
        case Script::ESCRIPTTOKEN_HEXINTEGER:
        case Script::ESCRIPTTOKEN_FLOAT:
	    case Script::ESCRIPTTOKEN_ENDOFLINENUMBER:
		case Script::ESCRIPTTOKEN_JUMP:
		case Script::ESCRIPTTOKEN_RUNTIME_MEMBERFUNCTION:
		case Script::ESCRIPTTOKEN_RUNTIME_CFUNCTION:
			p_token+=5;
            break;
        case Script::ESCRIPTTOKEN_VECTOR:
            p_token+=13;
            break;
        case Script::ESCRIPTTOKEN_PAIR:
            p_token+=9;
            break;
	    case Script::ESCRIPTTOKEN_STRING:
        case Script::ESCRIPTTOKEN_LOCALSTRING:
		{
            ++p_token;
			uint32 num_bytes=*p_token++;
			num_bytes+=(*p_token++)<<8;
			num_bytes+=(*p_token++)<<16;
			num_bytes+=(*p_token++)<<24;
            p_token+=num_bytes;
            break;
		}
		case Script::ESCRIPTTOKEN_CHECKSUM_NAME:
			// Skip over the token and checksum.
			p_token+=5;
			// Skip over the string.
			while (*p_token)
			{
				++p_token;
			}
			// Skip over the terminator.
			++p_token;
			break;
		case Script::ESCRIPTTOKEN_KEYWORD_RANDOM:
		case Script::ESCRIPTTOKEN_KEYWORD_RANDOM2:
		case Script::ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT:
		case Script::ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE:
		{
            ++p_token;

			uint32 num_jumps=*p_token++;
			num_jumps+=(*p_token++)<<8;
			num_jumps+=(*p_token++)<<16;
			num_jumps+=(*p_token++)<<24;

			// Skip over all the weight & jump offsets.
			p_token+=2*num_jumps+4*num_jumps;
			break;
		}

        default:
            Dbg_MsgAssert(0,("Unrecognized script token sent to SkipToken()"));
            break;
    }
    return p_token;
}


}

