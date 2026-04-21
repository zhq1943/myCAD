#include"extern/dxfsrc/libdxfrw.h"
#include"extern/dxfsrc/drw_interface.h"
#include<memory>
#include"LineSegment.h"
#include"Arc.h"

class MyDXFReader :public DRW_Interface {
public:
	void addLine(const DRW_Line& data) override {
		auto line = std::make_shared<LineSegment>("DXF_Line");
		line->Start = glm::vec3(data.basePoint.x, data.basePoint.y, data.basePoint.z);
		line->End = glm::vec3(data.secPoint.x, data.secPoint.y, data.secPoint.z);
		line->updateGeometry();
		tempObjects.push_back(line);
	}

	void addArc(const DRW_Arc& data) override {
		auto arc = std::make_shared<Arc>("DXF_Arc");
		arc->Center = glm::vec3(data.basePoint.x, data.basePoint.y, data.basePoint.z);
		arc->RadiusX = arc->RadiusY = data.radious;
		arc->StartAngle = glm::degrees(data.staangle);
		arc->EndAngle = glm::degrees(data.endangle);
		arc->updateGeometry();
		tempObjects.push_back(arc);
	}
	std::vector<std::shared_ptr<Geometry>> tempObjects;

	void addLayer(const DRW_Layer& data) override {
		// TODO: 实现添加 layer 的逻辑
	}

	//根据DRW_Interface的定义，补充其他抽象方法的实现
	void addHeader(const DRW_Header* data) override {
		// TODO: 实现添加 header 的逻辑
	}

	void addLType(const DRW_LType& data) override {
		// TODO: 实现添加 LType 的逻辑
	}

	void addDimStyle(const DRW_Dimstyle& data) override {
		// TODO: 实现添加 DimStyle 的逻辑
	}

	void addVport(const DRW_Vport& data) override {
		// TODO: 实现添加 Vport 的逻辑
	}

	void addTextStyle(const DRW_Textstyle& data) override {
		// TODO: 实现添加 TextStyle 的逻辑
	}

	void addAppId(const DRW_AppId& data) override {
		// TODO: 实现添加 AppId 的逻辑
	}

	void addBlock(const DRW_Block& data) override {
		// TODO: 实现添加 Block 的逻辑
	}

	void setBlock(const int handle) override {
		// TODO: 实现 setBlock 的逻辑
	}

	void endBlock() override {
		// TODO: 实现 endBlock 的逻辑
	}

	void addPoint(const DRW_Point& data) override {
		// TODO: 实现添加 Point 的逻辑
	}

	void addRay(const DRW_Ray& data) override {
		// TODO: 实现添加 Ray 的逻辑
	}

	void addXline(const DRW_Xline& data) override {
		// TODO: 实现添加 Xline 的逻辑
	}

	void addCircle(const DRW_Circle& data) override {
		// TODO: 实现添加 Circle 的逻辑
	}

	void addEllipse(const DRW_Ellipse& data) override {
		// TODO: 实现添加 Ellipse 的逻辑
	}

	void addLWPolyline(const DRW_LWPolyline& data) override {
		// TODO: 实现添加 LWPolyline 的逻辑
	}

	void addPolyline(const DRW_Polyline& data) override {
		// TODO: 实现添加 Polyline 的逻辑
	}

	void addSpline(const DRW_Spline* data) override {
		// TODO: 实现添加 Spline 的逻辑
	}

	void addKnot(const DRW_Entity& data) override {
		// TODO: 实现添加 Knot 的逻辑
	}

	void addInsert(const DRW_Insert& data) override {
		// TODO: 实现添加 Insert 的逻辑
	}

	void addTrace(const DRW_Trace& data) override {
		// TODO: 实现添加 Trace 的逻辑
	}

	void add3dFace(const DRW_3Dface& data) override {
		// TODO: 实现添加 3dFace 的逻辑
	}

	void addSolid(const DRW_Solid& data) override {
		// TODO: 实现添加 Solid 的逻辑
	}

	void addMText(const DRW_MText& data) override {
		// TODO: 实现添加 MText 的逻辑
	}

	void addText(const DRW_Text& data) override {
		// TODO: 实现添加 Text 的逻辑
	}

	void addDimAlign(const DRW_DimAligned *data) override {
		// TODO: 实现添加 DimAlign 的逻辑
	}

	void addDimLinear(const DRW_DimLinear *data) override {
		// TODO: 实现添加 DimLinear 的逻辑
	}

	void addDimRadial(const DRW_DimRadial *data) override {
		// TODO: 实现添加 DimRadial 的逻辑
	}

	void addDimDiametric(const DRW_DimDiametric *data) override {
		// TODO: 实现添加 DimDiametric 的逻辑
	}

	void addDimAngular(const DRW_DimAngular *data) override {
		// TODO: 实现添加 DimAngular 的逻辑
	}

	void addDimAngular3P(const DRW_DimAngular3p *data) override {
		// TODO: 实现添加 DimAngular3P 的逻辑
	}

	void addDimOrdinate(const DRW_DimOrdinate *data) override {
		// TODO: 实现添加 DimOrdinate 的逻辑
	}

	void addLeader(const DRW_Leader *data) override {
		// TODO: 实现添加 Leader 的逻辑
	}

	void addHatch(const DRW_Hatch *data) override {
		// TODO: 实现添加 Hatch 的逻辑
	}

	void addViewport(const DRW_Viewport& data) override {
		// TODO: 实现添加 Viewport 的逻辑
	}

	void addImage(const DRW_Image *data) override {
		// TODO: 实现添加 Image 的逻辑
	}

	void linkImage(const DRW_ImageDef *data) override {
		// TODO: 实现 linkImage 的逻辑
	}

	void addComment(const char* comment) override {
		// TODO: 实现添加 Comment 的逻辑
	}

	void writeHeader(DRW_Header& data) override {
		// TODO: 实现 writeHeader 的逻辑
	}

	void writeBlocks() override {
		// TODO: 实现 writeBlocks 的逻辑
	}

	void writeBlockRecords() override {
		// TODO: 实现 writeBlockRecords 的逻辑
	}

	void writeEntities() override {
		// TODO: 实现 writeEntities 的逻辑
	}

	void writeLTypes() override {
		// TODO: 实现 writeLTypes 的逻辑
	}

	void writeLayers() override {
		// TODO: 实现 writeLayers 的逻辑
	}

	void writeTextstyles() override {
		// TODO: 实现 writeTextstyles 的逻辑
	}

	void writeVports() override {
		// TODO: 实现 writeVports 的逻辑
	}

	void writeDimstyles() override {
		// TODO: 实现 writeDimstyles 的逻辑
	}

	void writeAppId() override {
		// TODO: 实现 writeAppId 的逻辑
	}
	
};

class DXFExportHandeler :public DRW_Interface {
public :
	std::vector<std::shared_ptr<Geometry>>* sceneObjects;
	dxfRW* mdxfRW;

	DXFExportHandeler(std::vector<std::shared_ptr<Geometry>>* objs, dxfRW* i_dxfRW) :sceneObjects(objs), mdxfRW(i_dxfRW){};

	void addLine(const DRW_Line& data) override {

	}

	void addArc(const DRW_Arc& data) override {

	}

	void addLayer(const DRW_Layer& data) override {
		// TODO: 实现添加 layer 的逻辑
	}

	//根据DRW_Interface的定义，补充其他抽象方法的实现
	void addHeader(const DRW_Header* data) override {
		// TODO: 实现添加 header 的逻辑
	}

	void addLType(const DRW_LType& data) override {
		// TODO: 实现添加 LType 的逻辑
	}

	void addDimStyle(const DRW_Dimstyle& data) override {
		// TODO: 实现添加 DimStyle 的逻辑
	}

	void addVport(const DRW_Vport& data) override {
		// TODO: 实现添加 Vport 的逻辑
	}

	void addTextStyle(const DRW_Textstyle& data) override {
		// TODO: 实现添加 TextStyle 的逻辑
	}

	void addAppId(const DRW_AppId& data) override {
		// TODO: 实现添加 AppId 的逻辑
	}

	void addBlock(const DRW_Block& data) override {
		// TODO: 实现添加 Block 的逻辑
	}

	void setBlock(const int handle) override {
		// TODO: 实现 setBlock 的逻辑
	}

	void endBlock() override {
		// TODO: 实现 endBlock 的逻辑
	}

	void addPoint(const DRW_Point& data) override {
		// TODO: 实现添加 Point 的逻辑
	}

	void addRay(const DRW_Ray& data) override {
		// TODO: 实现添加 Ray 的逻辑
	}

	void addXline(const DRW_Xline& data) override {
		// TODO: 实现添加 Xline 的逻辑
	}

	void addCircle(const DRW_Circle& data) override {
		// TODO: 实现添加 Circle 的逻辑
	}

	void addEllipse(const DRW_Ellipse& data) override {
		// TODO: 实现添加 Ellipse 的逻辑
	}

	void addLWPolyline(const DRW_LWPolyline& data) override {
		// TODO: 实现添加 LWPolyline 的逻辑
	}

	void addPolyline(const DRW_Polyline& data) override {
		// TODO: 实现添加 Polyline 的逻辑
	}

	void addSpline(const DRW_Spline* data) override {
		// TODO: 实现添加 Spline 的逻辑
	}

	void addKnot(const DRW_Entity& data) override {
		// TODO: 实现添加 Knot 的逻辑
	}

	void addInsert(const DRW_Insert& data) override {
		// TODO: 实现添加 Insert 的逻辑
	}

	void addTrace(const DRW_Trace& data) override {
		// TODO: 实现添加 Trace 的逻辑
	}

	void add3dFace(const DRW_3Dface& data) override {
		// TODO: 实现添加 3dFace 的逻辑
	}

	void addSolid(const DRW_Solid& data) override {
		// TODO: 实现添加 Solid 的逻辑
	}

	void addMText(const DRW_MText& data) override {
		// TODO: 实现添加 MText 的逻辑
	}

	void addText(const DRW_Text& data) override {
		// TODO: 实现添加 Text 的逻辑
	}

	void addDimAlign(const DRW_DimAligned* data) override {
		// TODO: 实现添加 DimAlign 的逻辑
	}

	void addDimLinear(const DRW_DimLinear* data) override {
		// TODO: 实现添加 DimLinear 的逻辑
	}

	void addDimRadial(const DRW_DimRadial* data) override {
		// TODO: 实现添加 DimRadial 的逻辑
	}

	void addDimDiametric(const DRW_DimDiametric* data) override {
		// TODO: 实现添加 DimDiametric 的逻辑
	}

	void addDimAngular(const DRW_DimAngular* data) override {
		// TODO: 实现添加 DimAngular 的逻辑
	}

	void addDimAngular3P(const DRW_DimAngular3p* data) override {
		// TODO: 实现添加 DimAngular3P 的逻辑
	}

	void addDimOrdinate(const DRW_DimOrdinate* data) override {
		// TODO: 实现添加 DimOrdinate 的逻辑
	}

	void addLeader(const DRW_Leader* data) override {
		// TODO: 实现添加 Leader 的逻辑
	}

	void addHatch(const DRW_Hatch* data) override {
		// TODO: 实现添加 Hatch 的逻辑
	}

	void addViewport(const DRW_Viewport& data) override {
		// TODO: 实现添加 Viewport 的逻辑
	}

	void addImage(const DRW_Image* data) override {
		// TODO: 实现添加 Image 的逻辑
	}

	void linkImage(const DRW_ImageDef* data) override {
		// TODO: 实现 linkImage 的逻辑
	}

	void addComment(const char* comment) override {
		// TODO: 实现添加 Comment 的逻辑
	}

	void writeHeader(DRW_Header& data) override {
		// TODO: 实现 writeHeader 的逻辑
		data.vars["$ACADVER"] = new DRW_Variant(1, "AC1015");
	}

	void writeBlocks() override {
		// TODO: 实现 writeBlocks 的逻辑
	}

	void writeBlockRecords() override {
		// TODO: 实现 writeBlockRecords 的逻辑
	}

	void writeEntities() override {
		// TODO: 实现 writeEntities 的逻辑
		for (auto& obj : *sceneObjects)
		{
			if (auto line = std::dynamic_pointer_cast<LineSegment>(obj))
			{
				DRW_Line dLine;
				dLine.basePoint.x = line->Start.x;
				dLine.basePoint.y = line->Start.y;
				dLine.basePoint.z = line->Start.z;
				dLine.secPoint.x = line->End.x;
				dLine.secPoint.y = line->End.y;
				dLine.secPoint.z = line->End.z;
				mdxfRW->writeLine(&dLine);
			}
			else if (auto arc = std::dynamic_pointer_cast<Arc>(obj))
			{
				DRW_Arc dArc;
				dArc.basePoint = DRW_Coord(arc->Center.x, arc->Center.y, arc->Center.z);
				dArc.radious = arc->RadiusX;
				dArc.staangle = arc->StartAngle;
				dArc.endangle = arc->EndAngle;
				mdxfRW->writeArc(&dArc);
			}
		}
	}

	void writeLTypes() override {
		// TODO: 实现 writeLTypes 的逻辑
	}

	void writeLayers() override {
		// TODO: 实现 writeLayers 的逻辑
		DRW_Layer layer;
		layer.name = "0";
		layer.color = 7;
		mdxfRW->writeLayer(&layer);
	}

	void writeTextstyles() override {
		// TODO: 实现 writeTextstyles 的逻辑
	}

	void writeVports() override {
		// TODO: 实现 writeVports 的逻辑
	}

	void writeDimstyles() override {
		// TODO: 实现 writeDimstyles 的逻辑
	}

	void writeAppId() override {
		// TODO: 实现 writeAppId 的逻辑
	}
};