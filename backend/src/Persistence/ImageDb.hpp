#pragma once

#include "oatpp-sqlite/orm.hpp"

#include "Dto/Image.hpp"

#include OATPP_CODEGEN_BEGIN(DbClient)

class ImageDb : public oatpp::orm::DbClient {
public:

  ImageDb(const std::shared_ptr<oatpp::orm::Executor>& executor)
    : oatpp::orm::DbClient(executor)
  {
    oatpp::orm::SchemaMigration migration(executor, "images");
    migration.addFile(1, DATABASE_MIGRATIONS "/001_images_init.sql");
    migration.migrate();

    auto version = executor->getSchemaVersion();
    OATPP_LOGD("ImageDb", "Migration - OK. Version=%lld.", version);
  }

  QUERY(enableForeignKey,
        "PRAGMA foreign_keys = ON;")

  QUERY(createImage,
        "INSERT INTO images"
        "("
          "algo, start_date, end_date, "
          "data, height, width, iterations, "
          "quality, time, user"
        ") VALUES "
        "("
          ":image.algo, :image.start_date, :image.end_date, "
          ":image.data, :image.height, :image.width, :image.iterations, "
          ":image.quality, :image.time, :image.user"
        ");",
        PARAM(oatpp::Object<Image>, image))

  QUERY(updateImage,
        "UPDATE images SET "
        " algo=:image.algo, "
        " start_date=:image.start_date, "
        " end_date=:image.end_date, "
        " data=:image.data, "
        " height=:image.height, "
        " width=:image.width, "
        " iterations=:image.iterations, "
        " quality=:image.quality, "
        " time=:image.time, "
        " user=:image.user "
        " WHERE id = :image.id;",
        PARAM(oatpp::Object<Image>, image))

  QUERY(getImagesByUser,
        "SELECT * FROM images WHERE user=:user;",
        PARAM(oatpp::Int32, user))

  QUERY(getImageById,
        "SELECT * FROM images WHERE id=:id;",
        PARAM(oatpp::Int32, id))

  QUERY(deleteImageById,
        "DELETE FROM images WHERE id=:id;",
        PARAM(oatpp::Int32, id))
};

#include OATPP_CODEGEN_END(DbClient)
