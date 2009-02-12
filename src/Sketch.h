// Sketch.h

#pragma once

#include "../interface/ObjList.h"
#include "../interface/HeeksColor.h"

enum SketchOrderType
{
	SketchOrderTypeEmpty,	// an empty sketch
	SketchOrderTypeOpen,	// a well ordered sketch, which doesn't have it's start and end touching
	SketchOrderTypeReverse,	// reverse the sketch, only used as a function, won't be returned by GetSketchOrder()
	SketchOrderTypeBad,		// "badly ordered" ( where the start of one span doesn't line up with the end of the previous span )
	SketchOrderTypeGood,	// a well ordered sketch, open/close unknown, won't be returned by GetSketchOrder()
	SketchOrderTypeCloseCW,	// a well ordered sketch, which is closed and clockwise
	SketchOrderTypeCloseCCW,// a well ordered sketch, which is closed and anti-clockwise
	MaxSketchOrderTypes
};

#if 0
have a "direction" drop-down in the
Sketch's properties. This would be "open" ( for a well ordered sketch ), "badly
ordered" ( where the start of one span doesn't line up with the end of the previous
span ), "closed cw" or "closed ccw" ( for well ordered sketches ). If the sketch is
well ordered, there will not be the "badly ordered" option available in the drop-down.
If the user selects an option, it will re-order the sketch.
#endif

class CSketch:public ObjList
{
	HeeksColor color;
	static wxIcon* m_icon;

	int GetClosedSketchTurningNumber(); //only use this for closed sketches
	double GetAngleAtJunction(HeeksObj* prev_object, HeeksObj* object);
	double GetAngleBetweenVectors(const gp_Vec& v0, const gp_Vec& v1, double prev_segment_curvature, double segment_curvature);
	int GetSegmentType(HeeksObj* object);
	double GetSegmentCurvature(HeeksObj* object);

public:
	static std::string m_sketch_order_str[MaxSketchOrderTypes];

	CSketch();
	CSketch(const CSketch& c);
	virtual ~CSketch();

	const CSketch& operator=(const CSketch& c);

	int GetType()const{return SketchType;}
	long GetMarkingMask()const{return MARKING_FILTER_SKETCH;}
	const wxChar* GetTypeString(void)const{return _("Sketch");}
	wxString GetIcon(){return _T("linedrawing");}
	void GetProperties(std::list<Property *> *list);
	HeeksObj *MakeACopy(void)const;
	void CopyFrom(const HeeksObj* object){operator=(*((CSketch*)((ObjList*)object)));}
	void WriteXML(TiXmlNode *root);
	bool UsesID(){return true;}
	void SetColor(const HeeksColor &col);
	const HeeksColor* GetColor()const;

	static HeeksObj* ReadFromXMLElement(TiXmlElement* pElem);

	SketchOrderType GetSketchOrder();
	bool ReOrderSketch(SketchOrderType new_order); // returns true if done
	void ReLinkSketch();
	void ReverseSketch();
};

class CSketchRelinker{
	const std::list<HeeksObj*> &m_old_list;
	std::set<HeeksObj*> m_added_from_old_set;
	std::list<HeeksObj*>::const_iterator m_old_front;
	std::list<HeeksObj*>::const_iterator m_old_back;
	HeeksObj* m_new_front;
	HeeksObj* m_new_back;
	bool AddNext();
	bool TryAdd(HeeksObj* object, bool front_not_back);

public:
	std::list<HeeksObj*> m_new_list;

	CSketchRelinker(const std::list<HeeksObj*>& old_list):m_old_list(old_list), m_new_front(NULL), m_new_back(NULL){}

	bool Do(); // makes m_new_list, returns false if there were some left over
};
