#include "skyBox.h"

void drawBox(bool text, std::vector<GLuint> textures)
{
    if(text)
    {
        glEnable(GL_TEXTURE_2D);
    }
    glPushMatrix();
        glTranslatef(0, 1, 0);
        glColor3f(1, 1, 1);

        TexCoord array[4] = {{1, 1}, {1, 0}, {0, 0}, {0, 1}};
        
        unsigned int index = 0;
        for (int i = 0; i < 6 && textures.size() >= 6; ++i) 
        {   
            int j = 0;
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glBegin(GL_QUADS);
            while(index < skyBox.size() && j < 4) 
            {
                glTexCoord2f(array[j].u, array[j].v);
                glVertex3f(skyBox[index].x, skyBox[index].y, skyBox[index].z);
                j++;
                index++;
            }
            glEnd();
        }
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}