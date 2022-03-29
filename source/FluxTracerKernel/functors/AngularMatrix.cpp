#include "AngularMatrix.h"
#include "math/1D/BinGrid.h"

using namespace cyi;

AngularMatrix::AngularMatrix(const vec3r &mainOpticalDirection, const Grid & gridAzmuth, const Grid& gridZenith)
    : m_gridAzimuth {gridAzmuth}, m_gridZenith {gridZenith}
{
    m_jp = mainOpticalDirection.normalized();
    m_ip = cross(m_jp, vec3r(0,1,0)).normalized();
    m_kp = cross(m_ip, m_jp);
    m_matrix.setDimensions(m_gridAzimuth.divisions(), m_gridZenith.divisions());
}

void AngularMatrix::process(const Ray &ray)
{
    vec3r direction = ray.vector().normalized();
    double azimuth = atan2( dot(direction, m_kp), dot(direction, m_ip));
    double zenith = acos(dot(direction, m_jp));
    int nAzimuth = m_gridAzimuth.index(azimuth);
    int nZenith = m_gridZenith.index(zenith);
    m_matrix(nAzimuth, nZenith) += ray.energy();

    const QString& h = ray.id;
    if (!m_results.contains(h))
        m_results[h].setDimensions(m_matrix.rows(), m_matrix.cols());
    m_results[h](nAzimuth, nZenith) += ray.energy();
}

void AngularMatrix::combine(const RayFunctor &rf)
{
    const AngularMatrix& other = dynamic_cast<const AngularMatrix&>(rf);
    m_matrix += other.m_matrix;

    for (QString& h : other.m_results.keys()) {
        if (!m_results.contains(h))
             m_results[h].setDimensions(m_matrix.rows(), m_matrix.cols());
        m_results[h] += other.m_results[h];

    }
}

static void saveMatrix(const QString &fileName, const Matrix<double>& matrix)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) return;
    QTextStream out(&file);

    // write x grid
//    for (int n = 0; n < m_xPoints.size(); ++n) {
//        out << m_xPoints[n];
//        if (n < m_xPoints.size() - 1)
//            out << ",";
//        else
//            out << endl;
//    }

    // write y grid
//    for (int m = 0; m < m_yPoints.size(); ++m) {
//        out << m_yPoints[m];
//        if (m < m_yPoints.size() - 1)
//            out << ",";
//        else
//            out << endl;
//    }

    // write energy
    for (int n = 0; n < matrix.rows(); ++n) {
        for (int m = 0; m < matrix.cols(); ++m) {
            out << matrix(n, m);
            if (m < matrix.cols() - 1)
                out << ",";
            else
                out << endl;
        }
    }
}

bool AngularMatrix::save(const QString &fileName) const
{
    saveMatrix(fileName, m_matrix);

    QFileInfo fileInfo(fileName);
    QString path = fileInfo.path();
    QString base = fileInfo.baseName();
    for (QString& h : m_results.keys()) {
        QString name = QString("%1/%2-%3.csv").arg(path).arg(base).arg(h);
        saveMatrix(name, m_results[h]);
     }

    return true;
}
