#include "gui_manager.h"


void GUIManager::init(int screenWidth, int screenHeight)
{
	m_fpsLabel = NULL;
    m_GUIComponentsFlags = 0;

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;

    // set m_GUIPipeline
    m_GUIPipeline.setMatrixMode(PROJECTION_MATRIX);
    m_GUIPipeline.loadIdentity();
    m_GUIPipeline.ortho(0, m_screenWidth, m_screenHeight, 0, -1, 1);

    m_GUIPipeline.setMatrixMode(MODEL_MATRIX);
    m_GUIPipeline.loadIdentity();

    m_textureQuad = QuadModel(1,1);

	
	char* filename = "gui/gui_renderer_data.json";

	Value vContent = utl::readJsonFileToVector(filename);
	const Array& vArray = vContent.get_array();

	string path = "gui_shaders/";

	Renderer::initRendererWrapper(vArray, &r_texture, "r_texture", path);
	Renderer::initRendererWrapper(vArray, &r_depthTexture, "r_depthTexture", path);
	Renderer::initRendererWrapper(vArray, &r_coloredRect, "r_coloredRect", path);
	Renderer::initRendererWrapper(vArray, &r_texturedRect, "r_texturedRect", path);
	Renderer::initRendererWrapper(vArray, &r_listBoxItemHighlight, "r_listBoxItemHighlight", path);
	Renderer::initRendererWrapper(vArray, &r_text, "r_text", path);

	Control::r_coloredRect = r_coloredRect;
	Control::r_texturedRect = r_texturedRect;
	Control::r_listBoxItemHighlight = r_listBoxItemHighlight;
	Control::m_textEngine.r_textRenderer = r_text;


	int xOffset = 55;
	int yOffset = 570;

	int BAR_WIDTH = 60;
	int BAR_HEIGHT = 10;

	xOffset = 0; yOffset = 0;
	m_fpsLabel = new Label("90", xOffset, yOffset, 50, 50, COLOR_GRAY);

	float width = 100;
	float height = 50;

	xOffset = 100;
	yOffset = 0;
	m_circleRadiusLabel = new Label("", xOffset, yOffset, width, height, COLOR_WHITE);
	m_circleRadiusLabel->setFont(15, COLOR_GREEN);
	
	xOffset = 250;
	m_circleCenterLabel = new Label("", xOffset, yOffset, width, height, COLOR_WHITE);
	m_circleCenterLabel->setFont(15, COLOR_GREEN);

	xOffset = 400;
	m_fillLabel = new Label("", xOffset, yOffset, width, height, COLOR_WHITE);
	m_fillLabel->setFont(15, COLOR_GREEN);

	xOffset = 550;
	m_addHalfRadiusLabel = new Label("", xOffset, yOffset, width, height, COLOR_WHITE);
	m_addHalfRadiusLabel->setFont(15, COLOR_GREEN);
	
	xOffset = 700;
	m_floatModeLabel = new Label("", xOffset, yOffset, width, height, COLOR_WHITE);
	m_floatModeLabel->setFont(15, COLOR_GREEN);


	addGUIComponent(m_fpsLabel);
	addGUIComponent(m_circleRadiusLabel);

	addGUIComponent(m_circleCenterLabel);
	addGUIComponent(m_fillLabel);
	addGUIComponent(m_addHalfRadiusLabel);
	addGUIComponent(m_floatModeLabel);

	utl::debug("GUI manager initing");
}

Pipeline& GUIManager::getPipeline()
{
	return m_GUIPipeline;
}


void GUIManager::setFPS(int fps)
{
	if (m_fpsLabel != NULL)
	{
		m_fpsLabel->setText(utl::intToStr(fps));
	}
}


void GUIManager::setCircleCenter(glm::vec2 center)
{
	if (m_circleCenterLabel != NULL)
	{
		m_circleCenterLabel->setText("center: " + utl::floatToStr(center.x) + ", " + utl::floatToStr(center.y));
	}
}

void GUIManager::setCircleRadius(float radius)
{
	if (m_fpsLabel != NULL)
	{
		m_circleRadiusLabel->setText("radius: " + utl::floatToStr(radius));
	}
}

void GUIManager::setFillFlag(bool flag)
{
	if (m_fillLabel != NULL)
	{
		if (flag)
		{
			m_fillLabel->setText("Filled (b)");
		}
		else
		{
			m_fillLabel->setText("Not Filled (b)");
		}
	}
}

void GUIManager::setHalfRadiusFlag(bool flag)
{
	if (m_addHalfRadiusLabel != NULL)
	{
		if (flag)
		{
			m_addHalfRadiusLabel->setText("add 0.5 to radius (m)");
		}
		else
		{
			m_addHalfRadiusLabel->setText("without 0.5 radius (m)");
		}
	}
}



void GUIManager::setFloatMode(bool flag)
{
	if (m_floatModeLabel != NULL)
	{
		if (flag)
		{
			m_floatModeLabel->setText("float Mode (n)");
		}
		else
		{
			m_floatModeLabel->setText("not in float mode (n)");
		}
	}
}
void GUIManager::initGUIRenderingSetup()
{
	setupRenderToScreen(0, 0, m_screenWidth, m_screenHeight);
}

void GUIManager::setupRenderToScreen(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void GUIManager::renderTextureFullScreen(GLuint textureId)
{
	renderTextureFullScreen(textureId, RENDER_TO_SCREEN);
}

void GUIManager::renderTextureFullScreen(GLuint textureId, GLuint fboTarget)
{
	renderTexture(textureId, fboTarget, 0, 0, m_screenWidth, m_screenHeight, r_texture);
}

void GUIManager::renderDepthTextureFullScreen(GLuint textureId)
{
	renderDepthTextureFullScreen(textureId, RENDER_TO_SCREEN);
}

void GUIManager::renderDepthTextureFullScreen(GLuint textureId, GLuint fboTarget)
{
	renderTexture(textureId, fboTarget, 0, 0, m_screenWidth, m_screenHeight, r_depthTexture);
}

void GUIManager::renderTexture(GLuint textureId, int x, int y, int width, int height, Renderer& r)
{
	renderTexture(textureId, RENDER_TO_SCREEN, x, y, width, height, r);
}

void GUIManager::renderTexture(GLuint textureId, GLuint fboTarget, int x, int y, int width, int height, Renderer& r)
{
	setupRenderToScreen(x, y, width, height);
	r.enableShader();
	r.setData(R_TEXTURE::u_texture, 0, GL_TEXTURE_2D, textureId);

    m_GUIPipeline.pushMatrix();
        m_GUIPipeline.translate(x, y, 0);
        m_GUIPipeline.scale(width, height, 1.0);

		r.setUniLocs(m_GUIPipeline);
        m_textureQuad.render();
    m_GUIPipeline.popMatrix();
	r.disableShader();
}


void GUIManager::renderTexture(GLuint TextureId, GLuint FboTarget, Rect rect)
{
    renderTexture(TextureId, FboTarget, rect.x, rect.y, rect.w, rect.h, r_texture);
}



void GUIManager::addGUIComponent(Control* control)
{
	control->setID(m_GUIComponentsID);
	m_GUIComponents.push_back(control);
}

void GUIManager::addDebugLabel(Control* control)
{
	control->setID(m_GUIComponentsID);
	m_debugLabels.push_back(control);
}

void GUIManager::removeDebugLabels()
{
	for (int i = 0; i < m_debugLabels.size(); i++)
	{
		delete m_debugLabels[i];
	}
	m_debugLabels.clear();
}

int GUIManager::getNumGUIComponent()
{
	return m_GUIComponents.size();
}

void GUIManager::updateAndRender(MouseState mouseState)
{
    for(int i=0; i<m_GUIComponents.size(); i++)
    {
		Control* control = m_GUIComponents[i];
		control->update(mouseState);
		control->render();
	}

	for (int i = 0; i<m_debugLabels.size(); i++)
	{
		Control* control = m_debugLabels[i];
		control->update(mouseState);
		control->render();
	}
}
