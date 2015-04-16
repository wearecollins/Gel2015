//
//  Svg2Mesh.h
//
//  Created by lars berg on 8/1/14.
//

#pragma once

#include "ofMain.h"
#include "ofxSvg.h"

namespace Svg2Mesh
{
	static void addSvgToMesh(string dir, ofMesh& m)
	{
		ofxSVG svg;
		string path = ofToDataPath(dir,true);
		ofLogVerbose() << "[addSvgToMesh] load "<<path;
		svg.load(path);
		
		for ( auto i=0; i<svg.getNumPath(); i++ )
		{
			auto& path = svg.getPathAt(i);
			path.simplify();
			
			auto pm = path.getTessellation();
			if(pm.getIndices().size())
			{
                auto& v = pm.getVertices();
                auto& c = pm.getColors();
                const auto & cp = svg.getPathAt(i).getFillColor();
                
                for(auto& i: pm.getIndices()){
                    m.addVertex(v[i]);
                    if ( c.size() > i )
                        m.addColor(c[i]);
                    else
                        m.addColor(cp);
                }
			}
			else
			{
                for(auto& v: pm.getVertices()){
                    m.addVertex(v);
                }
                for(auto& c: pm.getColors()){
                    m.addColor(c);
                }
			}
		}
	}
    
    static void addSvgToMeshVector(string dir, vector<ofMesh> & meshes)
    {
        ofxSVG svg;
        string path = ofToDataPath(dir,true);
        ofLogVerbose() << "[addSvgToMesh] load "<<path;
        svg.load(path);
        
        for ( auto i=0; i<svg.getNumPath(); i++ )
        {
            auto& path = svg.getPathAt(i);
            path.simplify();
            
            meshes.push_back(ofMesh());
            
            auto pm = path.getTessellation();
            if(pm.getIndices().size())
            {
                auto& v = pm.getVertices();
                auto& c = pm.getColors();
                const auto & cp = svg.getPathAt(i).getFillColor();
                
                for(auto& i: pm.getIndices()){
                    meshes.back().addVertex(v[i]);
                    if ( c.size() > i )
                        meshes.back().addColor(c[i]);
                    else
                        meshes.back().addColor(cp);
                }
            }
            else
            {
                for(auto& v: pm.getVertices()){
                    meshes.back().addVertex(v);
                }
                for(auto& c: pm.getColors()){
                    meshes.back().addColor(c);
                }
            }
        }
    }
}
