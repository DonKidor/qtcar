#include "mybrain.h"
#include <QRandomGenerator>

MyBrain::MyBrain()
{
    initNeurons();
    for(int i=0; i<m_connections.size(); ++i) {
        for(int j=0; j<m_connections[i].size(); ++j) {
            for(int k=0; k<m_connections[i][j].size(); ++k) {
                qreal f =QRandomGenerator::global()->generateDouble()-0.5;
                m_connections[i][j][k]=f*4;
            }
        }
    }
}

void MyBrain::initNeurons() {
    m_brainNeuron.resize(m_brainSize.size());
    for(int i=0; i<m_brainSize.size(); ++i) {
        m_brainNeuron[i].resize(m_brainSize[i]);
    }
    m_connections.resize(m_brainSize.size()-1);
    for(int i=1; i<m_brainSize.size(); ++i) {
        m_connections[i-1].resize(m_brainSize[i]-1);
        for(int j=0; j<m_brainSize[i]-1; ++j) {
            m_connections[i-1][j].resize(m_brainSize[i-1]);
        }
    }

}

MyBrain::MyBrain(const MyBrain &another) {
    initNeurons();
    for(int i=0; i<m_connections.size(); ++i) {
        for(int j=0; j<m_connections[i].size(); ++j) {
            for(int k=0; k<m_connections[i][j].size(); ++k) {
                m_connections[i][j][k]=another.m_connections[i][j][k];
            }
        }
    }

}


MyBrain::MyBrain(const MyBrain &another1, const MyBrain &another2) :m_brainNeuron(another1.m_brainNeuron.size()), m_connections(another1.m_connections.size()) {

    initNeurons();
    for(int i=0; i<m_connections.size(); ++i) {
        for(int j=0; j<m_connections[i].size(); ++j) {
            for(int k=0; k<m_connections[i][j].size(); ++k) {
                qreal x = QRandomGenerator::global()->generateDouble();
                if(x<0.5) {
                    m_connections[i][j][k]=another1.m_connections[i][j][k];
                } else {
                    m_connections[i][j][k]=another2.m_connections[i][j][k];
                }
            }
        }
    }

}

MyBrain& MyBrain::operator=(const MyBrain &another) {
    for(int i=0; i<m_connections.size(); ++i) {
        for(int j=0; j<m_connections[i].size(); ++j) {
            for(int k=0; k<m_connections[i][j].size(); ++k) {
                m_connections[i][j][k]=another.m_connections[i][j][k];
            }
        }
    }
    return *this;
}

MyBrain& MyBrain::mutate() {
    for(int i=0; i<m_connections.size(); ++i) {
        for(int j=0; j<m_connections[i].size(); ++j) {
            for(int k=0; k<m_connections[i][j].size(); ++k) {
                qreal x=QRandomGenerator::global()->generateDouble();
                if(x<=MUTATION_RATE) {
                    qreal f =QRandomGenerator::global()->generateDouble()-0.5;
                    qreal value = m_connections[i][j][k]+f;
                    if(value>2) value=2;
                    else if(value<-2) value=-2;
                    m_connections[i][j][k]=value;
                }
            }
        }
    }
    return *this;
}

QVector<bool> MyBrain::think(const QVector<qreal> &input) {
    for(int i=0; i<m_brainSize[0]; ++i) {
        m_brainNeuron[0][i]=input[i];
    }

    for(int i=1; i<m_brainSize.size(); ++i) {
        m_brainNeuron[i-1][m_brainNeuron[i-1].size()-1]=1;
        for(int j=0; j<m_connections[i-1].size(); ++j) {
            qreal res=0;
            for(int k=0; k<m_connections[i-1][j].size(); ++k) {
                res+=m_brainNeuron[i-1][k]*m_connections[i-1][j][k];
            }
            m_brainNeuron[i][j] = atan(res)/1.57;
        }
    }

    QVector<bool> res(4);
    for(int i=0; i<4; ++i) {
        res[i] = m_brainNeuron[m_brainSize.size()-1][i]>0.6;
    }
    return res;
}
