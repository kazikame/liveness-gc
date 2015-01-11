#include "SchemeAST.h"

#include <cassert>
#include <algorithm>

using namespace Scheme::AST;


std::ostream & IdExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                 bool with_label,
                                 Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    if (format == Scheme::output::SCHEME)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << *pID;
    else if(format == Scheme::output::PLAIN)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << *pID;
    else if(format == Scheme::output::JSON)
        stream << "{ \"type\": \"id\", \"value\": \"" << *pID << "\""
               << (with_label && label != "" ? std::string(", \"label\": \"") + label +"\"}" : "}");
    return stream;
}


std::ostream & ReturnExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                     bool with_label,
                                     Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    if (format == Scheme::output::SCHEME) {
        stream;
        pID->print(stream, 0, false, with_label, format);
    } else if(format == Scheme::output::PLAIN) {
        stream << "(RETURN ";
        pID->print(stream, 0, false, with_label, format) << ")";
    } else if(format == Scheme::output::JSON) {
        stream << "{";
        Scheme::output::indentTill(stream, depth + 2) << "\"type\": \"return\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"value\": ";
        pID->print(stream, depth + 2, false, with_label, format);
        Scheme::output::indentTill(stream, depth) << "}";
    }
    return stream;
}


std::ostream & NilConstExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                       bool with_label,
                                       Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    if(format == Scheme::output::SCHEME)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << "'()";
    if(format == Scheme::output::PLAIN)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << "NIL";
    else if(format == Scheme::output::JSON)
        stream << "{\"type\": \"nil\""
               << (with_label && label != "" ? std::string(", \"label\": \"") + label +"\"}" : "}");
    return stream;
}


std::ostream & IntConstExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                       bool with_label,
                                       Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    if(format == Scheme::output::SCHEME)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << *pIntVal;
    if(format == Scheme::output::PLAIN)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << *pIntVal;
    else if(format == Scheme::output::JSON)
        stream << "{ \"type\": \"iconst\", \"value\": " << *pIntVal
               << (with_label && label != "" ? std::string(", \"label\": \"") + label +"\"}" : "}");
    return stream;
}


std::ostream & StrConstExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                       bool with_label,
                                       Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    if(format == Scheme::output::SCHEME)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << *pStrVal;
    else if(format == Scheme::output::PLAIN)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << *pStrVal;
    else if(format == Scheme::output::JSON)
        stream << "{ \"type\": \"sconst\", \"value\": " << *pStrVal
               << (with_label && label != "" ? std::string(", \"label\": \"") + label +"\"}" : "}");
    return stream;
}


std::ostream & BoolConstExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                        bool with_label,
                                        Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    if(format == Scheme::output::SCHEME)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << (*pBoolVal ? "#t" : "#f");
    else if(format == Scheme::output::PLAIN)
        stream << (with_label ? (label + (label != "" ? ": " : "")) : "") << (*pBoolVal ? "#t" : "#f");
    else if(format == Scheme::output::JSON)
        stream << "{ \"type\": \"bconst\", \"value\": \"" << (*pBoolVal ? "#t\"" : "#f\"")
               << (with_label && label != "" ? std::string(", \"label\": \"") + label +"\"}" : "}");
    return stream;
}


std::ostream & IfExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                 bool with_label,
                                 Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    std::string labelstr = (with_label ?  (label + (label != "" ? ": " : "")) : "");
    unsigned labellen = labelstr.length();
    if(format == Scheme::output::SCHEME) {
        stream << labelstr << "(if ";
        pCond->print(stream, depth + 4 + labellen, false, with_label, format);
        pThen->print(stream, depth + 4 + labellen, true, with_label,  format);
        pElse->print(stream, depth + 4 + labellen, true, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::PLAIN) {
        stream << labelstr << "(IF ";
        pCond->print(stream, depth + 4 + labellen, false, with_label, format);
        pThen->print(stream, depth + 4 + labellen, true, with_label, format);
        pElse->print(stream, depth + 4 + labellen, true, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::JSON) {
        stream << "{";
        Scheme::output::indentTill(stream, depth + 2) << "\"type\": \"if\",";
        if(labellen != 0) Scheme::output::indentTill(stream,
                                                         depth + 2) << "\"label\": \"" << label << "\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"cond\": ";
        pCond->print(stream, depth + 2, false, with_label, format) << ",";
        Scheme::output::indentTill(stream, depth + 2) << "\"then\": ";
        pThen->print(stream, depth + 2, false, with_label, format) << ",";
        Scheme::output::indentTill(stream, depth + 2) << "\"else\": ";
        pElse->print(stream, depth + 2, false, with_label, format);
        Scheme::output::indentTill(stream, depth) << "}";
    }
    return stream;
}


std::ostream & LetExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                  bool with_label,
                                  Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    std::string labelstr = (with_label ?  (label + (label != "" ? ": " : "")) : "");
    unsigned labellen = labelstr.length();
    if(format == Scheme::output::SCHEME) {
        stream << labelstr << "(let ((";
        pID->print(stream, 0, false, with_label, format);
        stream << " ";
        pExpr->print(stream, depth + pID->getIDStr().length() + 8 + labellen, false, with_label,
                     format);
        stream << "))";
        pBody->print(stream, depth + 5 + labellen, true, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::PLAIN) {
        stream << labelstr << "(LET (";
        pID->print(stream, 0, false, with_label, format);
        stream << " = ";
        pExpr->print(stream, depth + pID->getIDStr().length() + 9 + labellen, false, with_label,
                     format);
        stream << ") IN";
        pBody->print(stream, depth + 5 + labellen, true, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::JSON) {
        stream << "{";
        Scheme::output::indentTill(stream, depth + 2) << "\"type\": \"let\",";
        if(labellen != 0)
            Scheme::output::indentTill(stream, depth + 2) << "\"label\": \"" << label << "\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"id\": ";
        pID->print(stream, depth + 2, false, with_label, format) << ",";
        Scheme::output::indentTill(stream, depth + 2) << "\"expr\": ";
        pExpr->print(stream, depth + 2, false, with_label, format) << ",";
        Scheme::output::indentTill(stream, depth + 2) << "\"body\": ";
        pBody->print(stream, depth + 2, false, with_label, format);
        Scheme::output::indentTill(stream, depth) << "}";
    }
    return stream;
}


std::ostream & UnaryPrimExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                        bool with_label,
                                        Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    std::string labelstr = (with_label ?  (label + (label != "" ? ": " : "")) : "");
    unsigned labellen = labelstr.length();
    if(format == Scheme::output::SCHEME) {
        stream << labelstr << "(" << node_name << " ";
        pArg->print(stream, depth + 1 + node_name.length() + 1 + labellen, false, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::PLAIN) {
        std::string name = node_name;
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        stream << labelstr << "(" << name << " ";
        pArg->print(stream, depth + 1 + node_name.length() + 1 + labellen, false, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::JSON) {
        stream << "{";
        Scheme::output::indentTill(stream, depth + 2) << "\"type\": \"prim\",";
        if(labellen != 0) Scheme::output::indentTill(stream,
                                                         depth + 2) << "\"label\": \"" << label << "\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"oper\": \"" << node_name << "\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"args\": [";
        pArg->print(stream, depth + 4, false, with_label, format);
        Scheme::output::indentTill(stream, depth + 2) << "]";
        Scheme::output::indentTill(stream, depth) << "}";
    }
    return stream;
}


std::ostream & BinaryPrimExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                         bool with_label,
                                         Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    std::string labelstr = (with_label ?  (label + (label != "" ? ": " : "")) : "");
    unsigned labellen = labelstr.length();
    if(format == Scheme::output::SCHEME) {
        stream << labelstr << "(" << node_name << " ";
        pArg1->print(stream, depth + 1 + node_name.length() + 1 + labellen, false, with_label, format);
        pArg2->print(stream, depth + 1 + node_name.length() + 1 + labellen, true, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::PLAIN) {
        stream << labelstr << "(" << node_name << " ";
        pArg1->print(stream, depth + 1 + node_name.length() + 1 + labellen, false, with_label, format);
        pArg2->print(stream, depth + 1 + node_name.length() + 1 + labellen, true, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::JSON) {
        stream << "{";
        Scheme::output::indentTill(stream, depth + 2) << "\"type\": \"prim\",";
        if(labellen != 0) Scheme::output::indentTill(stream,
                                                         depth + 2) << "\"label\": \"" << label << "\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"oper\": \"" << node_name << "\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"args\": [";
        pArg1->print(stream, depth + 4, true, with_label, format) << ",";
        pArg2->print(stream, depth + 4, true, with_label, format);
        Scheme::output::indentTill(stream, depth + 2) << "]";
        Scheme::output::indentTill(stream, depth) << "}";
    }
    return stream;
}


std::ostream & FuncExprNode::print(std::ostream & stream, unsigned depth, bool newline,
                                   bool with_label,
                                   Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    std::string labelstr = (with_label ?  (label + (label != "" ? ": " : "")) : "");
    unsigned labellen = labelstr.length();
    if(format == Scheme::output::SCHEME) {
        stream << labelstr << "(";
        pID->print(stream, depth + 1, false, with_label, format);
        if(pListArgs->size()) {
            stream << " ";
            std::list<ExprNode *>::const_iterator i = pListArgs->begin();
            (*i)->print(stream, depth + 1 + pID->getIDStr().length() + 1 + labellen, false, with_label,
                        format);
            for(++i; i != pListArgs->end(); ++i)
                (*i)->print(stream, depth + 1 + pID->getIDStr().length() + 1 + labellen, true, with_label,
                            format);
        }
        stream << ")";
    } else if(format == Scheme::output::PLAIN) {
        stream << labelstr << "(";
        pID->print(stream, depth + 1, false, with_label, format);
        if(pListArgs->size()) {
            stream << " ";
            std::list<ExprNode *>::const_iterator i = pListArgs->begin();
            (*i)->print(stream, depth + 1 + pID->getIDStr().length() + 1 + labellen, false, with_label,
                        format);
            for(++i; i != pListArgs->end(); ++i)
                (*i)->print(stream, depth + 1 + pID->getIDStr().length() + 1 + labellen, true, with_label,
                            format);
        }
        stream << ")";
    } else if(format == Scheme::output::JSON) {
        stream << "{";
        Scheme::output::indentTill(stream, depth + 2) << "\"type\": \"call\",";
        if(labellen != 0) Scheme::output::indentTill(stream,
                                                         depth + 2) << "\"label\": \"" << label << "\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"id\": ";
        pID->print(stream, depth + 2, false, with_label, format) << ",";
        Scheme::output::indentTill(stream, depth + 2) << "\"args\": [";
        for(std::list<ExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end(); ++i) {
            if(i != pListArgs->begin())  stream << ",";
            (*i)->print(stream, depth + 4, true, with_label, format);
        }
        Scheme::output::indentTill(stream, depth + 2) << "]";
        Scheme::output::indentTill(stream, depth) << "}";
    }
    return stream;
}


std::ostream & DefineNode::print(std::ostream & stream, unsigned depth, bool newline,
                                 bool with_label,
                                 Scheme::output::output_t format) const {
    Scheme::output::indentTill(stream, newline ? depth : 0);
    std::string labelstr = (with_label ? (label + (label != "" ? ": " : "")) : "");
    unsigned labellen = labelstr.length();
    if(format == Scheme::output::SCHEME) {
        stream << labelstr << "(define (";
        pID->print(stream, 0, false, with_label, format);
        for(std::list<IdExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end();
                ++i) {
            stream.put(' ');
            (*i)->print(stream, 0, false, with_label, format);
        }
        stream << ")";
        pExpr->print(stream, depth + 8 + labellen, true, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::PLAIN) {
        stream << labelstr << "(DEFINE (";
        pID->print(stream, 0, false, with_label, format);
        for(std::list<IdExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end();
                ++i) {
            stream.put(' ');
            (*i)->print(stream, 0, false, with_label, format);
        }
        stream << ")";
        pExpr->print(stream, depth + 8 + labellen, true, with_label, format);
        stream.put(')');
    } else if(format == Scheme::output::JSON) {
        stream << "{";
        Scheme::output::indentTill(stream, depth + 2) << "\"type\": \"define\",";
        if(labellen != 0) Scheme::output::indentTill(stream,
                                                         depth + 2) << "\"label\": \"" << label << "\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"id\": ";
        pID->print(stream, depth + 2, false, with_label, format) << ",";
        Scheme::output::indentTill(stream, depth + 2) << "\"args\": [";
        for(std::list<IdExprNode *>::const_iterator i = pListArgs->begin(); i != pListArgs->end();
                ++i) {
            if(i != pListArgs->begin())  stream << ",";
            (*i)->print(stream, depth + 4, true, with_label, format);
        }
        Scheme::output::indentTill(stream, depth + 2) << "],";
        Scheme::output::indentTill(stream, depth + 2) << "\"body\": ";
        pExpr->print(stream, depth + 2, false, with_label, format);
        Scheme::output::indentTill(stream, depth) << "}";
    }
    return stream;
}


std::ostream & ProgramNode::print(std::ostream & stream, unsigned depth, bool newline,
                                  bool with_label,
                                  Scheme::output::output_t format) const
{


    Scheme::output::indentTill(stream, newline ? depth : 0);
    if(format == Scheme::output::SCHEME) {
        for(std::list<DefineNode *>::const_iterator i = pListDefines->begin(); i != pListDefines->end();
                ++i) {
            (*i)->print(stream, depth, true, with_label, format);
            stream << "\n\n";
        }
        pExpr->print(stream, depth, true, with_label, format);
    } else if(format == Scheme::output::PLAIN) {
        stream << "PROGRAM:" << std::endl;
        for(std::list<DefineNode *>::const_iterator i = pListDefines->begin(); i != pListDefines->end();
                ++i) {
            (*i)->print(stream, depth, true, with_label, format);
            stream << "\n\n";
        }
        pExpr->print(stream, depth, true, with_label, format);
    } else if(format == Scheme::output::JSON) {
        stream << "{";
        Scheme::output::indentTill(stream, depth + 2) << "\"type\": \"program\",";
        Scheme::output::indentTill(stream, depth + 2) << "\"defines\": [";
        for(std::list<DefineNode *>::const_iterator i = pListDefines->begin(); i != pListDefines->end();
                ++i) {
            if(i != pListDefines->begin())  stream << ",";
            (*i)->print(stream, depth + 4, true, with_label, format);
        }
        Scheme::output::indentTill(stream, depth + 2) << "],";
        Scheme::output::indentTill(stream, depth + 2) << "\"body\": ";
        pExpr->print(stream, depth + 2, false, with_label, format);
        Scheme::output::indentTill(stream, depth) << "}";
    }

    return stream;
}




